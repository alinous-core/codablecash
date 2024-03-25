/*
 * UndoLogsPartDataFactory.cpp
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsPartDataFactory.h"

#include "trx/session/base/AbstractUndoLogPart.h"
namespace codablecash {

UndoLogsPartDataFactory::UndoLogsPartDataFactory() {
}

UndoLogsPartDataFactory::~UndoLogsPartDataFactory() {
}

IBlockObject* UndoLogsPartDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return AbstractUndoLogPart::createFromBinary(in);
}

AbstractBtreeDataFactory* UndoLogsPartDataFactory::copy() const noexcept {
	return new UndoLogsPartDataFactory();
}

} /* namespace codablecash */
