/*
 * UndoInsertRecordPart.cpp
 *
 *  Created on: 2023/07/27
 *      Author: iizuka
 */

#include "trx/session/record/UndoInsertRecordPart.h"

#include "schema_table/table_store/TableStore.h"

#include "engine/CdbOid.h"


namespace codablecash {

UndoInsertRecordPart::UndoInsertRecordPart(const UndoInsertRecordPart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INSERT_RECORD) {
	this->recordOid = inst.recordOid;
}

UndoInsertRecordPart::UndoInsertRecordPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INSERT_RECORD) {
	this->recordOid = 0;
}

UndoInsertRecordPart::~UndoInsertRecordPart() {

}

int UndoInsertRecordPart::binarySize() const {
	int total = sizeof(uint8_t);

	total += sizeof(this->recordOid);

	return total;
}

void UndoInsertRecordPart::toBinary(ByteBuffer *out) const {
	out->put(type);
	out->putLong(this->recordOid);
}

void UndoInsertRecordPart::fromBinary(ByteBuffer *in) {
	this->recordOid = in->getLong();
}

IBlockObject* UndoInsertRecordPart::copyData() const noexcept {
	return new UndoInsertRecordPart(*this);
}

void UndoInsertRecordPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	CdbOid oid(this->recordOid);

	store->remove(&oid);
}

void UndoInsertRecordPart::setRecordOid(uint64_t oid) noexcept {
	this->recordOid = oid;
}

} /* namespace codablecash */
