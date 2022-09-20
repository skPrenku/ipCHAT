#include "../common/refs.h"
#include <map>

using port_t = short;

struct clientInfo {
	clientInfo() = default;
	clientInfo(std::string_view clUserName , SOCKET s) : userid(clUserName), clSocket(s) {	
	}
	std::string userid;
	SOCKET clSocket = INVALID_SOCKET;
};

std::map<SOCKET, clientInfo> clientList;
int acceptClient(SOCKET s);
// a very basic server
int main() {
	WsaWrapper wsa;

	int rc;
	port_t port = 3232; // some random port

	socket_t s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); assert(s > 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	rc = bind(s, (sockaddr*)&addr, sizeof(addr)); assert(rc == 0);
	rc = listen(s, 8);

	char buf[128];
	socklen_t buflen = sizeof(buf);
	socket_t cli = accept(s, (sockaddr*)buf, &buflen);
	if (cli != INVALID_SOCKET)
		acceptClient(cli);

	closesocket(cli);
	closesocket(s);
}

int acceptClient(SOCKET s)
{
	char buffer[16];
	ZeroMemory(&buffer, 16);
	int bytes = recv(s, buffer, sizeof(buffer), 0);
	if (bytes >)
	{
		std::string userID = buffer;

		clientList[s] = clientInfo(userID, s);
		printf("User %s accepted on socket: %d",userID.data(), (int)s);
		bytes = send(s, "OK", 2, 0);
		return 0;
	}
	else {
		printf("User not accepted\n");
	}

}