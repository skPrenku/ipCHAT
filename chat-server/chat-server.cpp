#include "../common/refs.h"
#include <map>
#include <thread>

using port_t = short;

struct clientInfo {
	clientInfo() = default;
	clientInfo(std::string_view clUserName , SOCKET s) : userid(clUserName), clSocket(s) {	
	}
	std::string userid;
	SOCKET clSocket = INVALID_SOCKET;
};

std::map<SOCKET, clientInfo> clientList;
std::vector<std::thread> workers;

int acceptClient(SOCKET s);

void* newClThread(SOCKET s, std::string_view cl_name);
void msg_to_all(char* buffer,SOCKET s);

// a very basic server
int main() {
	WsaWrapper wsa;

	int rc;
	port_t port = 3232; // some random port

	socket_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); assert(s > 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	rc = bind(s, (sockaddr*)&addr, sizeof(addr)); assert(rc == 0);
	rc = listen(s, 8);

	char buf[128];
	socklen_t buflen = sizeof(buf);
	while (serverIsRunning) {
		socket_t cli = accept(s, (sockaddr*)buf, &buflen);		
		if (cli != INVALID_SOCKET)
			acceptClient(cli);
	}

	closesocket(s);
}

int acceptClient(SOCKET s)
{
	char buffer[16];
	ZeroMemory(&buffer, 16);
	int bytes = recv(s, buffer, sizeof(buffer), 0);
	if (bytes > 0)
	{
		std::string userID = buffer;

		clientList[s] = clientInfo(userID, s);
		printf("User %s accepted on socket: %d\n",userID.data(), (int)s);
		bytes = send(s, "OK", 2, 0);
		workers.push_back(std::thread(newClThread, s, userID.data()));
		

	}
	else {
		printf("User not accepted\n");
	}

	return -1;
}

void* newClThread(SOCKET s, std::string_view cl_name)
{
	char buffer[128];
	while (serverIsRunning) {


		int nBytes = recv(s, buffer, 128, 0);
		//if buffer = "-list" the server should list all users... to this one.
		if (nBytes > 0)
			msg_to_all(buffer,s);
		if (nBytes == 0)
			printf("Client [%s] disconnected.\n", cl_name.data());

	}
	return 0;
}

void msg_to_all(char* buffer,SOCKET s)
{



	for (auto it = clientList.begin(); it != clientList.end(); ++it)
	{
		if (it->first != s)
		{
			send(it->first, buffer, sizeof(buffer), 0);
			printf("sending msg to: %s\n", it->second.userid.c_str());
		}


	}

}