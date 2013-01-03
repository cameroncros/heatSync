/*
 * Watcher.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#include "Watcher.h"
#include "File.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <boost/thread.hpp>

class File;




Watcher::Watcher(Share &shr, Database &db, std::string path) {
	this->shr = &shr;
	this->db = &db;
	this->path = path;
	event = (inotify_event*)malloc(sizeof(struct inotify_event) + NAME_MAX + 1);
	watchfile = inotify_init();
	boost::thread trd(work);
	trd.join();
	// TODO Auto-generated constructor stub

}

void Watcher::work(long int asdf) {
	File *nFile;

	inotify_add_watch(watchfile,path.c_str(),IN_CLOSE_WRITE|IN_ATTRIB|IN_CREATE|IN_DELETE|IN_MODIFY|IN_DELETE|IN_DELETE_SELF|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
	while(true) {
		std::string newpath = path;
		newpath.append(event->name);
		read(watchfile, event, sizeof(struct inotify_event) + NAME_MAX + 1);
		nFile = new File(newpath);
		db->insertFileDetails(*nFile, *shr);
		delete(nFile);
		std::cout << event->name << std::endl;
	}
}

Watcher::~Watcher() {
	delete(trd);
	free(event);
	close(watchfile);
	// TODO Auto-generated destructor stub
}

