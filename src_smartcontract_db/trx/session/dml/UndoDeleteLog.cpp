/*
 * UndoDeleteLog.cpp
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#include "trx/session/dml/UndoDeleteLog.h"

namespace codablecash {

UndoDeleteLog::UndoDeleteLog(const UndoDeleteLog &inst) : AbstractUndoDmlLog(inst) {
}

UndoDeleteLog::UndoDeleteLog() : AbstractUndoDmlLog(AbstractUndoLog::UNDO_TYPE_DELETE) {

}

UndoDeleteLog::~UndoDeleteLog() {

}

int UndoDeleteLog::binarySize() const {
	int total = AbstractUndoDmlLog::binarySize();

	return total;
}

void UndoDeleteLog::toBinary(ByteBuffer *out) const {
	AbstractUndoDmlLog::toBinary(out);
}

void UndoDeleteLog::fromBinary(ByteBuffer *in) {
	AbstractUndoDmlLog::fromBinary(in);
}

IBlockObject* UndoDeleteLog::copyData() const noexcept {
	return new UndoDeleteLog(*this);
}

} /* namespace codablecash */
