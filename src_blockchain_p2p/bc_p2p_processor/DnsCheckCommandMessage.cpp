/*
 * DnsCheckCommandMessage.cpp
 *
 *  Created on: Feb 2, 2025
 *      Author: iizuka
 */

#include "bc_p2p_processor/DnsCheckCommandMessage.h"
#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "bc_p2p_cmd_network/NodeNetworkInfo.h"

#include "bc/ISystemLogger.h"
#include "bc/ExceptionThrower.h"

#include "pubsub/CommandPublisher.h"
#include "pubsub/PubsubNetworkException.h"

#include "ipconnect/IClientSocket.h"
#include "ipconnect/StackConnectionCloser.h"

#include "bc_p2p_info/P2pDnsManager.h"

#include "base/StackRelease.h"

namespace codablecash {

DnsCheckCommandMessage::DnsCheckCommandMessage() {
	this->nodeNetInfo = nullptr;
}

DnsCheckCommandMessage::~DnsCheckCommandMessage() {
	delete this->nodeNetInfo;
}

void DnsCheckCommandMessage::setNodeInfo(const NodeNetworkInfo *info) {
	delete this->nodeNetInfo;
	this->nodeNetInfo = new NodeNetworkInfo(*info);
}

void DnsCheckCommandMessage::execute(ICommandParameter *param) {
	P2pRequestProcessor* processor = dynamic_cast<P2pRequestProcessor*>(param);
	ISystemLogger* logger = processor->getLogger();

	const UnicodeString* host = this->nodeNetInfo->getHost();
	if(host != nullptr){
		checkAndRegister(processor, logger);
	}
}

void DnsCheckCommandMessage::checkAndRegister(P2pRequestProcessor* processor, ISystemLogger* logger) {
	{
		IClientSocket* con = this->nodeNetInfo->getConnection(); __STP(con);
		StackConnectionCloser _st_closer(con);

		ExceptionThrower<PubsubNetworkException>::throwExceptionIfCondition(!con->isConnected(), L"Connection does not reach.", __FILE__, __LINE__);
	}

	// dns check
	P2pDnsManager* dnsManager = processor->getP2pDnsManager();
	dnsManager->updateOrAddRecord(this->nodeNetInfo, logger);
}

} /* namespace codablecash */
