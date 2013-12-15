/*
 * SyncServer.h
 *
 *  Created on: Jan 26, 2013
 *      Author: cameron
 */

#ifndef SYNCSERVER_H_
#define SYNCSERVER_H_

#include "Sync.h"
class SecureConnection;

class SyncServer : public Sync {
private:
	SecureConnection *conn;
	bool connected;
public:
	SyncServer(SecureConnection *);
	virtual ~SyncServer();
	bool isConnected() const;
};

#endif /* SYNCSERVER_H_ */
