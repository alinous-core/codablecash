/*
 * StackClientListnerRemover.cpp
 *
 *  Created on: 2023/11/13
 *      Author: iizuka
 */

#include "bc_p2p_client/StackClientListnerRemover.h"
#include "bc_p2p_client/P2pClient.h"

#include "bc_p2p_cmd_client_notify/IClientNotifyListner.h"

#include <cassert>

namespace codablecash {

StackClientListnerRemover::StackClientListnerRemover(P2pClient* client, IClientNotifyListner* listner) {
	this->client = client;
	this->listner = listner;

	this->client->addListner(listner);
}

StackClientListnerRemover::~StackClientListnerRemover() {
	bool bl = this->client->removeListner(this->listner);

	assert(bl == true);
}

} /* namespace codablecash */
