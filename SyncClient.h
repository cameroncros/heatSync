/*
 * SyncClient.h
 *
 *  Created on: Jan 26, 2013
 *      Author: cameron
 */

#ifndef SYNCCLIENT_H_
#define SYNCCLIENT_H_

#include "Sync.h"
class SecureConnection;
class Share;

class SyncClient : public Sync {
private:
	SecureConnection *conn;
	Share *share;
	bool connected;
public:
	SyncClient(SecureConnection *, Share *);
	virtual ~SyncClient();
	bool isConnected() const;
};

#endif /* SYNCCLIENT_H_ */
