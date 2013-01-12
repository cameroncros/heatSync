/*
 * SecureConnection.h
 *
 *  Created on: Jan 11, 2013
 *      Author: cameron
 */

#ifndef SECURECONNECTION_H_
#define SECURECONNECTION_H_

#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

class SecureConnection {
private:
	int sock;
	SSL_CTX *sslContext;
	SSL *secure;
public:
	SecureConnection(char *);
	SecureConnection(int);
	virtual ~SecureConnection();
	void sendData(void *, int);
	void getData(void *, int);

};

#endif /* SECURECONNECTION_H_ */
