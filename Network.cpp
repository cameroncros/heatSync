/*
 * Network.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cameron
 */

#include "Network.h"
#include "Avahi.h"
#include <iostream>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <string.h>



Network::Network() {
	sockSetup();
	ava = new Avahi();
	int sk;
	while (true) {
		sk = sockListen();
		connections[sk] = new SecureConnection(sk);
	}
	// TODO Auto-generated constructor stub

}

Network::~Network() {
	if (listenSock) {
		close(listenSock);
	}
	delete(ava);
	// TODO Auto-generated destructor stub
}

void Network::sockSetup() {

	listenSock = 0;
	int out, yes=1;
	struct addrinfo hints, *servinfo = NULL,*p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((out = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") getaddrinfo: " << gai_strerror(out) << std::endl;
		return;
	}
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((listenSock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			close(listenSock);
			listenSock = 0;
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") socket: " << strerror(errno) << std::endl;
		}
		if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") setsockopt" << strerror(errno) << std::endl;
			close(listenSock);
			listenSock = 0;
			return;
		}
		if (bind(listenSock, p->ai_addr, p->ai_addrlen) == -1) {
			close(listenSock);
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") bind" << strerror(errno) << std::endl;
			continue;
		} else {
			break;
		}
	}
	freeaddrinfo(servinfo);
	if (p == NULL) {
		if (listenSock) {
			close(listenSock);
			listenSock = 0;
		}
		std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") Failed Connection: " << strerror(errno) << std::endl;
		throw std::exception();
	}
}

int Network::sockListen() {
	int newSock = -1;
	while (newSock == -1) {
		if (listen(listenSock,100) == -1) {
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") Failed Connection: " << strerror(errno) << std::endl;	\
			break;
		}
		newSock = accept(listenSock, NULL, NULL);
		if (newSock == -1) {
			std::cerr << "(" << __FILE__ << ":" << __LINE__ << ") Failed Connection: " << strerror(errno) << std::endl;	\
		}
	}
	return newSock;
}

