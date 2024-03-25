/*
 * UndoUpdateLog.cpp
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#include "trx/session/dml/UndoUpdateLog.h"

namespace codablecash {

UndoUpdateLog::UndoUpdateLog(const UndoUpdateLog &inst) : AbstractUndoDmlLog(inst) {
}

UndoUpdateLog::UndoUpdateLog() : AbstractUndoDmlLog(AbstractUndoLog::UNDO_TYPE_UPDATE) {

}

UndoUpdateLog::~UndoUpdateLog() {

}

int UndoUpdateLog::binarySize() const {
	int total = AbstractUndoDmlLog::binarySize();

	return total;
}

void UndoUpdateLog::toBinary(ByteBuffer *out) const {
	AbstractUndoDmlLog::toBinary(out);
}

void UndoUpdateLog::fromBinary(ByteBuffer *in) {
	AbstractUndoDmlLog::fromBinary(in);
}

IBlockObject* UndoUpdateLog::copyData() const noexcept {
	return new UndoUpdateLog(*this);
}

} /* namespace codablecash */
