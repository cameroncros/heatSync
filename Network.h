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
#include <thread>

extern AvahiSimplePoll *simple_publish_poll;
extern AvahiSimplePoll *simple_discovery_poll;
extern AvahiEntryGroup *group;
extern std::string name;

#define PORT 19669

class Network {
private:
	AvahiClient *publishClient;
	AvahiClient *discoveryClient;
	AvahiServiceBrowser *sb;
	char hostname[HOST_NAME_MAX];
	std::thread *pubtrd;
	std::thread *distrd;

	struct timeval tv;

public:
	Network();
	virtual ~Network();
	void avahiDiscovery();
	void avahiPublish();

	void create_services(AvahiClient *c);

};

#endif /* NETWORK_H_ */
