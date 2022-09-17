// chat-client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../common/refs.h"


using port_t = short;

// a very simple client
int main() {
	WsaWrapper wsa;

	int rc;
	port_t port = 3232; // some random port

	socket_t s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); assert(s > 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	rc = connect(s, (sockaddr*)&addr, sizeof(addr)); assert(rc == 0);

	ssize_t nbytes;
	nbytes = send(s, "dan hello, how are you?", 23, 0); assert(nbytes == 23);
	char buffer[128];
	nbytes = recv(s, buffer, sizeof(buffer), 0);

	closesocket(s);
}
