/*
 * Network.cpp
 *
 *  Created on: Jan 8, 2013
 *      Author: cameron
 */

#include "Avahi.h"
#include "Host.h"

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-client/publish.h>


#include <avahi-common/error.h>
#include <avahi-common/malloc.h>
#include <avahi-common/alternative.h>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <string>
#include <thread>
#include <map>
#include <unistd.h>

AvahiSimplePoll *simple_publish_poll;
AvahiSimplePoll *simple_discovery_poll;
AvahiEntryGroup *group;
std::string name;

std::map<std::string, Host *> avahiHosts;

Avahi::Avahi() {
	discoveryClient = NULL;
	publishClient = NULL;
	sb = NULL;
	simple_publish_poll = NULL;
	name = "heatSync";
	gethostname(hostname, sizeof(hostname));
	name.append(hostname);

	pubtrd = new std::thread(&Avahi::avahiDiscovery, this);
	distrd = new std::thread(&Avahi::avahiPublish, this);

}

Avahi::~Avahi() {
	if (discoveryClient) {
		avahi_client_free(discoveryClient);
	}

	if (publishClient) {
		avahi_client_free(publishClient);
	}

	if (simple_discovery_poll) {
		avahi_simple_poll_free(simple_discovery_poll);
	}

	if (sb) {
		avahi_service_browser_free(sb);
	}
	if (simple_publish_poll) {
		avahi_simple_poll_free(simple_publish_poll);
	}
	distrd->join();
	pubtrd->join();

	// TODO Auto-generated destructor stub
}
/*browse Callbacks*/
void client_publish_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata);
void client_discovery_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata);
void browse_callback(
		AvahiServiceBrowser *b,
		AvahiIfIndex interface,
		AvahiProtocol protocol,
		AvahiBrowserEvent event,
		const char *name,
		const char *type,
		const char *domain,
		AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
		void* userdata);
void resolve_callback(
		AvahiServiceResolver *r,
		AVAHI_GCC_UNUSED AvahiIfIndex interface,
		AVAHI_GCC_UNUSED AvahiProtocol protocol,
		AvahiResolverEvent event,
		const char *name,
		const char *type,
		const char *domain,
		const char *host_name,
		const AvahiAddress *address,
		uint16_t port,
		AvahiStringList *txt,
		AvahiLookupResultFlags flags,
		AVAHI_GCC_UNUSED void* userdata);
/* publish callbacks */
void create_services(AvahiClient *c);
void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata);



void Avahi::avahiDiscovery() {
	int error;

	/* Allocate main loop object */
	if (!(simple_discovery_poll = avahi_simple_poll_new())) {
		std::cerr << "Failed to create simple poll object." << std::endl;
	}

	/* Allocate a new client */
	discoveryClient = avahi_client_new(avahi_simple_poll_get(simple_discovery_poll), (AvahiClientFlags)0, client_discovery_callback, NULL, &error);

	/* Check whether creating the client object succeeded */
	if (!discoveryClient) {
		std::cerr << "Failed to create client: %s\n" <<  avahi_strerror(error) << std::endl;
	}

	/* Create the service browser */
	if (!(sb = avahi_service_browser_new(discoveryClient, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, "_heatSync._tcp", NULL, (AvahiLookupFlags)0, browse_callback, discoveryClient))) {
		std::cerr << "Failed to create service browser: %s\n" << avahi_strerror(avahi_client_errno(discoveryClient)) << std::endl;
	}
	/* Run the main loop */
	avahi_simple_poll_loop(simple_discovery_poll);
}

void Avahi::avahiPublish() {
	int error;

	/* Allocate main loop object */
	if (!(simple_publish_poll = avahi_simple_poll_new())) {
		std::cerr << "Failed to create simple poll object." << std::endl;
	}

	/* Allocate a new client */
	publishClient = avahi_client_new(avahi_simple_poll_get(simple_publish_poll), (AvahiClientFlags)0, client_publish_callback, NULL, &error);

	/* Check whether creating the client object succeeded */
	if (!publishClient) {
		std::cerr << "Failed to create client: %s\n" <<  avahi_strerror(error) << std::endl;
	}

	/* Run the main loop */
	avahi_simple_poll_loop(simple_publish_poll);
}

void client_discovery_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {
	assert(c);

	/* Called whenever the client or server state changes */
	switch (state) {
	case AVAHI_CLIENT_S_RUNNING:

		/* The server has startup successfully and registered its host
		 * name on the network, so it's time to create our services */
		//create_services(c);
		break;

	case AVAHI_CLIENT_FAILURE:

		std::cerr << "Client failure: " << avahi_strerror(avahi_client_errno(c)) << std::endl;
		avahi_simple_poll_quit(simple_discovery_poll);

		break;

	case AVAHI_CLIENT_S_COLLISION:

		/* Let's drop our registered services. When the server is back
		 * in AVAHI_SERVER_RUNNING state we will register them
		 * again with the new host name. */

	case AVAHI_CLIENT_S_REGISTERING:

		/* The server records are now being established. This
		 * might be caused by a host name change. We need to wait
		 * for our own records to register until the host name is
		 * properly esatblished. */

		if (group)
			avahi_entry_group_reset(group);

		break;

	case AVAHI_CLIENT_CONNECTING:
		;
		break;
	}
}

void client_publish_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {
	assert(c);

	/* Called whenever the client or server state changes */
	switch (state) {
	case AVAHI_CLIENT_S_RUNNING:

		/* The server has startup successfully and registered its host
		 * name on the network, so it's time to create our services */
		create_services(c);
		break;

	case AVAHI_CLIENT_FAILURE:

		std::cerr << "Client failure: " << avahi_strerror(avahi_client_errno(c)) << std::endl;
		avahi_simple_poll_quit(simple_publish_poll);

		break;

	case AVAHI_CLIENT_S_COLLISION:

		/* Let's drop our registered services. When the server is back
		 * in AVAHI_SERVER_RUNNING state we will register them
		 * again with the new host name. */

	case AVAHI_CLIENT_S_REGISTERING:

		/* The server records are now being established. This
		 * might be caused by a host name change. We need to wait
		 * for our own records to register until the host name is
		 * properly esatblished. */

		if (group)
			avahi_entry_group_reset(group);

		break;

	case AVAHI_CLIENT_CONNECTING:
		;
		break;
	}
}

void browse_callback(
		AvahiServiceBrowser *b,
		AvahiIfIndex interface,
		AvahiProtocol protocol,
		AvahiBrowserEvent event,
		const char *name,
		const char *type,
		const char *domain,
		AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
		void* userdata) {

	AvahiClient *c = (AvahiClient *)userdata;
	assert(b);

	/* Called whenever a new services becomes available on the LAN or is removed from the LAN */

	switch (event) {
	case AVAHI_BROWSER_FAILURE:

		std::cerr << "(Browser) " << avahi_strerror(avahi_client_errno(avahi_service_browser_get_client(b))) << std::endl;
		avahi_simple_poll_quit(simple_publish_poll);
		return;

	case AVAHI_BROWSER_NEW:
		std::cerr << "(Browser) NEW: service '" << name << "' of type '" << type << "' in domain '" << domain << "'\n" << std::endl;

		/* We ignore the returned resolver object. In the callback
               function we free it. If the server is terminated before
               the callback function is called the server will free
               the resolver for us. */

		if (!(avahi_service_resolver_new(c, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, (AvahiLookupFlags)0, resolve_callback, c)))
			std::cerr << "Failed to resolve service '" << name << "': " << avahi_strerror(avahi_client_errno(c)) <<std::endl;

		break;

	case AVAHI_BROWSER_REMOVE:
		delete(avahiHosts[name]);
		avahiHosts.erase(name);
		std::cerr << "(Browser) REMOVE: service '" << name << "' of type '" << type << "' in domain '" << domain << std::endl;
		break;

	case AVAHI_BROWSER_ALL_FOR_NOW:
	case AVAHI_BROWSER_CACHE_EXHAUSTED:
		std::cerr << "(Browser) " << (event == AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" : "ALL_FOR_NOW") << std::endl;
		break;
	}
}

void resolve_callback(
		AvahiServiceResolver *r,
		AVAHI_GCC_UNUSED AvahiIfIndex interface,
		AVAHI_GCC_UNUSED AvahiProtocol protocol,
		AvahiResolverEvent event,
		const char *name,
		const char *type,
		const char *domain,
		const char *host_name,
		const AvahiAddress *address,
		uint16_t port,
		AvahiStringList *txt,
		AvahiLookupResultFlags flags,
		AVAHI_GCC_UNUSED void* userdata) {

	assert(r);

	/* Called whenever a service has been resolved successfully or timed out */

	switch (event) {
	case AVAHI_RESOLVER_FAILURE:
		std::cerr << "(Resolver) Failed to resolve service '" << name <<"' of type '" << type << "' in domain '" << domain << "': " << avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(r))) << std::endl;
		break;

	case AVAHI_RESOLVER_FOUND: {
		char a[AVAHI_ADDRESS_STR_MAX], *t;

		if (!(flags & AVAHI_LOOKUP_RESULT_LOCAL)) {
			std::cerr << "Service '" << name << "' of type '" << type << "' in domain '" << domain << "':" << std::endl;

			avahi_address_snprint(a, sizeof(a), address);
			t = avahi_string_list_to_string(txt);
			std::cerr << host_name << ":" <<port << "(" << a << ")" << std::endl;
			std::cerr << "TXT=" << t << std::endl;
			std::cerr << "cookie is " << avahi_string_list_get_service_cookie(txt) << std::endl;
			std::cerr << "is_local: " << !!(flags & AVAHI_LOOKUP_RESULT_LOCAL) << std::endl;
			std::cerr << "our_own: " << !!(flags & AVAHI_LOOKUP_RESULT_OUR_OWN) << std::endl;
			std::cerr << "wide_area: " << !!(flags & AVAHI_LOOKUP_RESULT_WIDE_AREA) << std::endl;
			std::cerr << "multicast: " << !!(flags & AVAHI_LOOKUP_RESULT_MULTICAST) << std::endl;
			std::cerr << "cached: " << !!(flags & AVAHI_LOOKUP_RESULT_CACHED) << std::endl;

			avahiHosts[name] = new Host((char *)name, (char *)host_name, port, (char *)t);
			avahi_free(t);
			//todo:create new host here;
		}



	}
	}

	avahi_service_resolver_free(r);
}

void create_services(AvahiClient *c) {
	char r[128];
	int ret;
	assert(c);

	/* If this is the first time we're called, let's create a new
	 * entry group if necessary */

	if (!group)
		if (!(group = avahi_entry_group_new(c, entry_group_callback, NULL))) {
			std::cerr << "avahi_entry_group_new() failed: " << avahi_strerror(avahi_client_errno(c)) << std::endl;
		}

	/* If the group is empty (either because it was just created, or
	 * because it was reset previously, add our entries.  */

	if (avahi_entry_group_is_empty(group)) {
		std::cerr << "Adding service '" << name << "'" << std::endl;

		/* Create some random TXT data */
		snprintf(r, sizeof(r), "random=%i", rand());

		/* We will now add two services and one subtype to the entry
		 * group. The two services have the same name, but differ in
		 * the service type (IPP vs. BSD LPR). Only services with the
		 * same name should be put in the same entry group. */

		if ((ret = avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, (AvahiPublishFlags)0, name.c_str(), "_heatSync._tcp", NULL, NULL, PORTINT, "version=0.1", NULL, NULL )) < 0) { //todo: magically put in version no.
			if (ret == AVAHI_ERR_COLLISION) {
				name = avahi_alternative_service_name(name.c_str());

				std::cerr << "Service name collision, renaming service to '" << name << "'" << std::endl;

				avahi_entry_group_reset(group);

				create_services(c);
				return;
			}

			std::cerr << "Failed to add " << name << " service: " << avahi_strerror(ret) << std::endl;
		}

		/* Tell the server to register the service */
		if ((ret = avahi_entry_group_commit(group)) < 0) {
			std::cerr << "Failed to commit entry group: " << avahi_strerror(ret) << std::endl;
		}
	}

	avahi_simple_poll_quit(simple_publish_poll);
}

void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata) {
	assert(g == group || group == NULL);
	group = g;

	/* Called whenever the entry group state changes */

	switch (state) {
	case AVAHI_ENTRY_GROUP_ESTABLISHED :
		/* The entry group has been established successfully */
		std::cerr << "Service '" << name << "' successfully established." << std::endl;
		break;

	case AVAHI_ENTRY_GROUP_COLLISION : {
		/* A service name collision with a remote service
		 * happened. Let's pick a new name */
		name = avahi_alternative_service_name(name.c_str());


		std::cerr << "Service name collision, renaming service to '" << name << "'" << std::endl;

		/* And recreate the services */
		create_services(avahi_entry_group_get_client(g));
		break;
	}

	case AVAHI_ENTRY_GROUP_FAILURE :

		std::cerr << "Entry group failure: " << avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))) << std::endl;

		/* Some kind of failure happened while we were registering our services */
		avahi_simple_poll_quit(simple_publish_poll);
		break;

	case AVAHI_ENTRY_GROUP_UNCOMMITED:
	case AVAHI_ENTRY_GROUP_REGISTERING:
		break;
	}
}

