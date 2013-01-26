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
	std::string port;
	std::string version;

public:
	Host(char *nm, char *hostn, char *prt, char *vers);
	virtual ~Host();

	const std::string& getHostname() const;
	void setHostname(const std::string& hostname);
	const std::string& getName() const;
	void setName(const std::string& name);
	const std::string& getVersion() const;
	void setVersion(const std::string& version);
	const std::string& getPort() const;
	void setPort(const std::string& port);
};

#endif /* HOST_H_ */
