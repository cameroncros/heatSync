/*
 * host.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef HOST_H_
#define HOST_H_

#include <string>
#include <thread>

#include "SecureConnection.h"

class Host {
private:
	std::string name;
	std::string hostname;
	int port;
	std::string version;
	SecureConnection *conn;
	std::thread *trd;

public:
	Host(char *nm, char *hostn, int prt, char *vers);
	virtual ~Host();
	void sync();
};

#endif /* HOST_H_ */
