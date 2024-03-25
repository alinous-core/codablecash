/*
 * UndoLogsKeyFactory.cpp
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsKeyFactory.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogsPartIdKey.h"

namespace codablecash {

UndoLogsKeyFactory::UndoLogsKeyFactory() {

}

UndoLogsKeyFactory::~UndoLogsKeyFactory() {

}

AbstractBtreeKey* UndoLogsKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == UNDO_LOGS_ID_KEY){
		return UndoLogsIdkey::fromBinary(in);
	}
	else if(keyType == UNDO_LOGS_PART_ID_KEY){
		return UndoLogsPartIdKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* UndoLogsKeyFactory::copy() const noexcept {
	return new UndoLogsKeyFactory();
}

} /* namespace codablecash */
