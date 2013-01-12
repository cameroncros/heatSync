/*
 * SecureConnection.cpp
 *
 *  Created on: Jan 11, 2013
 *      Author: cameron
 */

#include "SecureConnection.h"
#include "Network.h"

#include <netdb.h>
#include <cstring>
#include <iostream>
#include <errno.h>
#include <unistd.h>




SecureConnection::SecureConnection(char *ad) {
	sock = 0;
	secure = NULL;
	sslContext = NULL;

	int out;
	struct addrinfo hints, *servinfo = NULL, *p= NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((out = getaddrinfo(ad, PORT, &hints, &servinfo)) != 0) {
		std::cerr << "(Secure Connection) getaddrinfo: " << gai_strerror(out) << std::endl;
		return;
	}
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			close(sock);
			sock = 0;
			std::cerr << "(Secure Connection) socket: " << strerror(errno) << std::endl;
		}
		if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock);
			sock = 0;
			std::cerr << "(Secure Connection) socket: " << strerror(errno) << std::endl;
		} else {
			break;
		}
	}
	freeaddrinfo(servinfo);
	if (p == NULL) {
		if (sock) {
			close(sock);
			sock = 0;
		}
		std::cerr << "(Secure Connection) Failed Connection: " << strerror(errno) << std::endl;
		return;
	}
	//finished creating a socket, now add the ssl part(curtesy of: http://savetheions.com/2010/01/16/quickly-using-openssl-in-c/)
	SSL_load_error_strings();
	SSL_library_init();
	if ((sslContext = SSL_CTX_new(SSLv23_client_method ())) == NULL) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if ((secure = SSL_new(sslContext)) == NULL) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if (!SSL_set_fd(secure, sock)) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if (SSL_connect (secure) != 1) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}


	// TODO Auto-generated constructor stub

}

SecureConnection::SecureConnection(int sk) {
	sock = sk;
	secure = NULL;
	sslContext = NULL;


	//finished creating a socket, now add the ssl part(curtesy of: http://savetheions.com/2010/01/16/quickly-using-openssl-in-c/)
	SSL_load_error_strings();
	SSL_library_init();
	if ((sslContext = SSL_CTX_new(SSLv23_client_method ())) == NULL) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if ((secure = SSL_new(sslContext)) == NULL) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if (!SSL_set_fd(secure, sock)) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}
	if (SSL_connect (secure) != 1) {
		std::cerr << "(Secure Connection) SSL: " << ERR_error_string(errno, NULL) << std::endl;
		return;
	}


	// TODO Auto-generated constructor stub

}

SecureConnection::~SecureConnection() {
	if (sock) {
		close(sock);
	}
	if (secure) {
		SSL_shutdown (secure);
		SSL_free(secure);
	}
	if (sslContext) {
		SSL_CTX_free(sslContext);
	}
	// TODO Auto-generated destructor stub
}

void SecureConnection::sendData(void *data, int size) {

}

void SecureConnection::getData(void *file, int size) {
	;
}
