/*
 * UndoInsertLog.cpp
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */

#include "trx/session/dml/UndoInsertLog.h"

namespace codablecash {

UndoInsertLog::UndoInsertLog(const UndoInsertLog &inst) : AbstractUndoDmlLog(inst) {
}

UndoInsertLog::UndoInsertLog() : AbstractUndoDmlLog(AbstractUndoLog::UNDO_TYPE_INSERT) {

}

UndoInsertLog::~UndoInsertLog() {

}

int UndoInsertLog::binarySize() const {
	int total = AbstractUndoDmlLog::binarySize();

	return total;
}

void UndoInsertLog::toBinary(ByteBuffer *out) const {
	AbstractUndoDmlLog::toBinary(out);
}

void UndoInsertLog::fromBinary(ByteBuffer *in) {
	AbstractUndoDmlLog::fromBinary(in);
}

IBlockObject* UndoInsertLog::copyData() const noexcept {
	return new UndoInsertLog(*this);
}

} /* namespace codablecash */
