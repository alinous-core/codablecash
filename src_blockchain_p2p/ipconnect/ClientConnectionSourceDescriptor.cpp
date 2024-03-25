/*
 * ClientConnectionSourceDescriptor.cpp
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#include "ipconnect/ClientConnectionSourceDescriptor.h"

#include "base/UnicodeString.h"

namespace codablecash {

ClientConnectionSourceDescriptor::ClientConnectionSourceDescriptor(const ClientConnectionSourceDescriptor &inst) {
	this->afFamily = inst.afFamily;
	this->host = new UnicodeString(inst.host);
	this->port = inst.port;
}

ClientConnectionSourceDescriptor::ClientConnectionSourceDescriptor(uint8_t family, const UnicodeString* host, int port) {
	this->afFamily = family;
	this->host = new UnicodeString(host);
	this->port = port;
}

ClientConnectionSourceDescriptor::~ClientConnectionSourceDescriptor() {
	delete this->host;
}

} /* namespace codablecash */
