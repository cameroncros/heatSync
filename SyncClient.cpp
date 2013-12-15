/*
 * SyncClient.cpp
 *
 *  Created on: Jan 26, 2013
 *      Author: cameron
 */

#include "SyncClient.h"
#include "SecureConnection.h"

#include <string.h>
#include <iostream>

SyncClient::SyncClient(SecureConnection *sec, Share *shr) {
	conn = sec;
	connected = 1;
	share = shr;
	std::string message = "Hello?";
	std::cout << "Connected and sent initial messages" << std::endl;
	conn->sendData((void *)message.c_str(), sizeof(message));
	// TODO Auto-generated constructor stub

}

bool SyncClient::isConnected() const {
	return connected;
}

SyncClient::~SyncClient() {
	// TODO Auto-generated destructor stub
}

