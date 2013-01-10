/*
 * host.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef HOST_H_
#define HOST_H_

#include <string>

class Host {
private:
	std::string name;
	std::string hostname;
	int port;
	std::string version;

public:
	Host(std::string name, std::string hostn, int port, std::string version);
	virtual ~Host();
};

#endif /* HOST_H_ */
