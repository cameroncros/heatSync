/*
 * host.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Host.h"

Host::Host(char *nm, char *hostn, char *prt, char *vers) {
	name = nm;
	hostname = hostn;
	port = prt;
	version = vers;
	// TODO Auto-generated constructor stub

}

Host::~Host() {
	// TODO Auto-generated destructor stub
}

const std::string& Host::getHostname() const {
	return hostname;
}

void Host::setHostname(const std::string& hostname) {
	this->hostname = hostname;
}

const std::string& Host::getName() const {
	return name;
}

void Host::setName(const std::string& name) {
	this->name = name;
}

const std::string& Host::getVersion() const {
	return version;
}

const std::string& Host::getPort() const {
	return port;
}

void Host::setPort(const std::string& port) {
	this->port = port;
}

void Host::setVersion(const std::string& version) {
	this->version = version;
}

