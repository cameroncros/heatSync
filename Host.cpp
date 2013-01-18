/*
 * host.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Host.h"

Host::Host(char *nm, char *hostn, int prt, char *vers) {
	name = nm;
	hostname = hostn;
	port = prt;
	version = vers;
	char port[6];
	sprintf(port, "%i", prt);
	conn= new SecureConnection(hostn, port);
	trd = new std::thread(&Host::sync, this);
	// TODO Auto-generated constructor stub

}

Host::~Host() {
	delete(conn);
	trd->join();
	// TODO Auto-generated destructor stub
}

void Host::sync() {

}
