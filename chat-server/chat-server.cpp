#include "../common/refs.h"
#include <map>

using port_t = short;

struct clientInfo {
	clientInfo() = default;
	clientInfo(std::string_view clUserName) {
		userid = clUserName;
	}
	std::string userid;
};

std::map<SOCKET, clientInfo> clientList;

// a very basic server
int main() {
	WsaWrapper wsa;

	int rc;
	port_t port = 3232; // some random port

	socket_t s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); assert(s > 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	rc = bind(s, (sockaddr*)&addr, sizeof(addr)); assert(rc == 0);
	rc = listen(s, 8);

	char buf[128];
	socklen_t buflen = sizeof(buf);
	socket_t cli = accept(s, (sockaddr*)buf, &buflen);

	ssize_t nbytes;
	nbytes = recv(cli, buf, sizeof(buf), 0);
	// examine what was sent
	nbytes = send(cli, "jan ok", 6, 0); assert(nbytes == 6);

	closesocket(cli);
	closesocket(s);
}

int acceptClient(SOCKET s)
{
	char buffer[16];
	int bytes = recv(s, buffer, sizeof(buffer), 0);
	if (buffer > 0)
	{
		std::string_view userID(buffer, sizeof(buffer));

		clientList[s] = clientInfo( userID);
	}
}