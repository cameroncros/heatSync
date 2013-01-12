/*
 * Avahi.h
 *
 *  Created on: Jan 8, 2013
 *      Author: cameron
 */

#ifndef AVAHI_H_
#define AVAHI_H_

#include "Network.h"

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

class Avahi {
private:
	AvahiClient *publishClient;
	AvahiClient *discoveryClient;
	AvahiServiceBrowser *sb;
	char hostname[HOST_NAME_MAX];
	std::thread *pubtrd;
	std::thread *distrd;

	struct timeval tv;

public:
	Avahi();
	virtual ~Avahi();
	void avahiDiscovery();
	void avahiPublish();

	void create_services(AvahiClient *c);

};

#endif /* AVAHI_H_ */
