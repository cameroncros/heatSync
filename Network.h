/*
 * Network.h
 *
 *  Created on: Jan 11, 2013
 *      Author: cameron
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#define PORT "19669"
#define PORTINT 19669

#include <vector>

#include "Avahi.h"
#include "SecureConnection.h"

class Avahi;
class SecureConnection;
class SyncServer;

class Network {
private:
	int listenSock;
	Avahi *ava;
	SSL_CTX *sslContext;
	std::vector<SyncServer> syncServers;
public:
	Network();
	virtual ~Network();
	void loadCertificates();
	void sockSetup();
	int sockListen();
};

#endif /* NETWORK_H_ */
