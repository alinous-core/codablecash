/*
 * AbstractCentralProcessorCommandMessage.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "bc_processor/AbstractCentralProcessorCommandMessage.h"

#include "bc_processor/CentralProcessor.h"

namespace codablecash {

AbstractCentralProcessorCommandMessage::AbstractCentralProcessorCommandMessage() {
	this->processor = nullptr;
}

AbstractCentralProcessorCommandMessage::~AbstractCentralProcessorCommandMessage() {
	this->processor = nullptr;
}

void AbstractCentralProcessorCommandMessage::execute(ICommandParameter *param) {
	this->processor = dynamic_cast<CentralProcessor*>(param);

	process(this->processor);
}

} /* namespace codablecash */
