// chat-server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <assert.h>

// Copied for work we've done before
#if defined(_WIN32) || defined(_WIN64)
// Windows
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#define  _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "AdvApi32.lib")

class WsaWrapper {
public:
	WsaWrapper() {
		WSADATA data;
		auto rc = WSAStartup(MAKEWORD(2, 2), &data);
		assert(rc == 0);
	}
	~WsaWrapper() {
		WSACleanup();
	}
};
using socket_t = SOCKET;
using ssize_t = int;

#else
// Unix
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

using socket_t = int;
#endif

using port_t = short;

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
