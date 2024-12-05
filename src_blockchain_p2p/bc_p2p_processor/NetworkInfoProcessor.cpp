/*
 * NetworkInfoProcessor.cpp
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#include "bc_p2p_processor/NetworkInfoProcessor.h"
#include "bc_p2p_processor/P2pRequestProcessor.h"

#include "procesor_multi/MultipleCommandProcessor.h"

#include "base/UnicodeString.h"

namespace codablecash {

NetworkInfoProcessor::NetworkInfoProcessor(P2pRequestProcessor* processor, int numThreads, ISystemLogger* logger) {
	this->processor = processor;
	this->numThreads = numThreads;
	this->logger = logger;

	this->executor = nullptr;

	this->serial = 0;
}

NetworkInfoProcessor::~NetworkInfoProcessor() {
	shutdown();
}

void NetworkInfoProcessor::start() {
	if(this->executor == nullptr){
		UnicodeString name(L"");

		const UnicodeString* nodeName = this->processor->getNodeName();
		if(nodeName != nullptr){
			name.append(nodeName);
			name.append(L"_");
		}
		name.append(THREAD_NAME);

		this->executor = new MultipleCommandProcessor(this->processor, this->numThreads, this->logger, &name);
		this->executor->start();
	}
}

void NetworkInfoProcessor::shutdown() {
	if(this->executor != nullptr){
		this->executor->shutdown();
		delete this->executor;
		this->executor = nullptr;
	}
}

} /* namespace codablecash */
