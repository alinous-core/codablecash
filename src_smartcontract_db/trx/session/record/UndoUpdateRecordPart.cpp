/*
 * UndoUpdateRecordPart.cpp
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#include "trx/session/record/UndoUpdateRecordPart.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

#include "schema_table/table_store/TableStore.h"
namespace codablecash {

UndoUpdateRecordPart::UndoUpdateRecordPart(const UndoUpdateRecordPart &inst) : AbstractUndoLogPart(PART_UNDO_UPDATE_RECORD) {
	this->record = inst.record != nullptr ? dynamic_cast<CdbRecord*>(inst.record->copyData()) : nullptr;
}

UndoUpdateRecordPart::UndoUpdateRecordPart() : AbstractUndoLogPart(PART_UNDO_UPDATE_RECORD) {
	this->record = nullptr;
}

UndoUpdateRecordPart::~UndoUpdateRecordPart() {
	delete this->record;
}

void UndoUpdateRecordPart::setRecord(const CdbRecord *record) noexcept {
	delete this->record;
	this->record = dynamic_cast<CdbRecord*>(record->copyData());
}

int UndoUpdateRecordPart::binarySize() const {
	BinaryUtils::checkNotNull(this->record);

	int total = sizeof(uint8_t);

	total += this->record->binarySize();

	return total;
}

void UndoUpdateRecordPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->record);

	out->put(type);
	this->record->toBinary(out);
}

void UndoUpdateRecordPart::fromBinary(ByteBuffer *in) {
	static CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(in); __STP(obj);
	this->record = dynamic_cast<CdbRecord*>(obj);
	BinaryUtils::checkNotNull(this->record);

	__STP_MV(obj);
}

IBlockObject* UndoUpdateRecordPart::copyData() const noexcept {
	return new UndoUpdateRecordPart(*this);
}

void UndoUpdateRecordPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	store->remove(this->record->getOid());
	store->insert(this->record);
}

} /* namespace codablecash */
