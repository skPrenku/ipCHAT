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