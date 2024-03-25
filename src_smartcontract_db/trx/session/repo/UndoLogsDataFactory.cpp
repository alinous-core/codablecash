/*
 * UndoLogsDataFactory.cpp
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsDataFactory.h"
#include "trx/session/base/AbstractUndoLog.h"


namespace codablecash {

UndoLogsDataFactory::UndoLogsDataFactory() {

}

UndoLogsDataFactory::~UndoLogsDataFactory() {

}

IBlockObject* UndoLogsDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return AbstractUndoLog::createFromBinary(in);
}

AbstractBtreeDataFactory* UndoLogsDataFactory::copy() const noexcept {
	return new UndoLogsDataFactory();
}

} /* namespace codablecash */
