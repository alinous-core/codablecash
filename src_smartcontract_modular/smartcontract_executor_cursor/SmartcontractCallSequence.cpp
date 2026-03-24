/*
 * SmartcontractCallSequence.cpp
 *
 *  Created on: Mar 22, 2026
 *      Author: iizuka
 */

#include "smartcontract_executor_cursor/SmartcontractCallSequence.h"
#include "smartcontract_executor_cursor/SessionContextCursorData.h"


namespace codablecash {

SmartcontractCallSequence::SmartcontractCallSequence() {
	this->list = new ArrayList<SessionContextCursorData>();
}

SmartcontractCallSequence::~SmartcontractCallSequence() {
	this->list->deleteElements();
	delete this->list;
}

void SmartcontractCallSequence::addFirst(const SessionContextCursorData *data) {
	SessionContextCursorData* d = dynamic_cast<SessionContextCursorData*>(data->copyData());

	this->list->addElement(d, 0);
}

} /* namespace codablecash */
