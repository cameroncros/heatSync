/*
 * SyncServer.cpp
 *
 *  Created on: Jan 26, 2013
 *      Author: cameron
 */

#include "SyncServer.h"

#include "SecureConnection.h"

#include <iostream>

SyncServer::SyncServer(SecureConnection *sec) {
	conn = sec;
	connected = 1;

	char temp[1024];
	int out = -1;
	conn->getData(temp, &out);
	std::cout << "Received " << out << " bytes containing:" << temp <<std::endl;
	// TODO Auto-generated constructor stub

}

bool SyncServer::isConnected() const {
	return connected;
}

SyncServer::~SyncServer() {
	// TODO Auto-generated destructor stub
}

