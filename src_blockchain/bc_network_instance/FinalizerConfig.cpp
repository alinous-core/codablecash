/*
 * FinalizerConfig.cpp
 *
 *  Created on: 2023/12/09
 *      Author: iizuka
 */

#include "bc_network_instance/FinalizerConfig.h"

#include "bc_network/NodeIdentifierSource.h"

namespace codablecash {

FinalizerConfig::FinalizerConfig(const FinalizerConfig &inst) {
	this->voterSource = inst.voterSource != nullptr ? dynamic_cast<NodeIdentifierSource*>(inst.voterSource->copyData()) : nullptr;
}

FinalizerConfig::FinalizerConfig() {
	this->voterSource = nullptr;
}

FinalizerConfig::~FinalizerConfig() {
	delete this->voterSource;
}

void FinalizerConfig::setVoterSource(const NodeIdentifierSource *source) noexcept {
	delete this->voterSource;
	this->voterSource = dynamic_cast<NodeIdentifierSource*>(source->copyData());
}

} /* namespace codablecash */
