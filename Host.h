/*
 * host.h
 *
 *  Created on: Dec 12, 2012
 *      Author: cameron
 */

#ifndef HOST_H_
#define HOST_H_

#include <vector>

#include "Share.h"
class Share;

class Host {
private:
	int version;
	std::vector<Share> shares;
public:
	Host();
	virtual ~Host();
};

#endif /* HOST_H_ */
