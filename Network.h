/*
 * Network.h
 *
 *  Created on: Jan 8, 2013
 *      Author: cameron
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-client/publish.h>
#include <avahi-common/simple-watch.h>
#include <string>
#include <limits.h>

extern AvahiSimplePoll *simple_poll;
extern AvahiEntryGroup *group;
extern std::string name;

#define PORT 19669

class Network {
private:
	AvahiClient *client;
	AvahiServiceBrowser *sb;
	char hostname[HOST_NAME_MAX];

	struct timeval tv;

public:
	Network();
	virtual ~Network();
	void avahiDiscovery();

	void create_services(AvahiClient *c);

};

#endif /* NETWORK_H_ */
