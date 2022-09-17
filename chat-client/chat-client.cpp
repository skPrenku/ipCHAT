// chat-client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "defs.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>

void sendClientHello(socket_t s, std::string_view userid);

// a very simple client
int main(int argc, char* argv[])
try {
#if defined(_WIN32) || defined(_WIN64)
	WsaWrapper wsa;
#endif

	const char* userid = (argc > 1 ? argv[1] : "emily");
	int rc;
	port_t port = 3232; // some random port

	socket_t s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); assert(s > 0);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	rc = connect(s, (sockaddr*)&addr, sizeof(addr)); assert(rc == 0);

	sendClientHello(s, userid);

	ssize_t nbytes;
	nbytes = send(s, "dan hello, how are you?", 23, 0); assert(nbytes == 23);
	char buffer[128];
	nbytes = recv(s, buffer, sizeof(buffer), 0);

#if defined(_WIN32) || defined(_WIN64)
	closesocket(s);
#else
	close(s);
#endif
}
catch (const std::exception& e) {
	std::cerr << "fatal: " << e.what() << "\n";
}

// send "Client Hello" message, send on initial connect
void sendClientHello(socket_t s, std::string_view userid) {
	std::clog << "sendClientHello: sending userid=" << userid << '\n';
	ssize_t nbytes = send(s, userid.data(), userid.size(), 0);
	if (nbytes != static_cast<ssize_t>(userid.size())) {
		throw std::runtime_error("cannot send ClientHello");
	}

	const std::string_view expectedServerReply{ "OK" };
	char buffer[16];
	nbytes = recv(s, buffer, sizeof(buffer), 0);
	std::clog << "sendClientHello: received nbytes=" << nbytes << '\n';
	if (nbytes > 0) {
		std::clog << "sendClientHello: received=" << std::string_view{ buffer, static_cast<std::size_t>(nbytes) } << '\n';
	}
	if (nbytes != 2 || expectedServerReply != std::string_view{ buffer, 2 }) {
		throw std::runtime_error("failed to receive ClientHello ack");
	}
}
