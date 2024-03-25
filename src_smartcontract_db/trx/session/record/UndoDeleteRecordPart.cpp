/*
 * UndoDeleteRecordPart.cpp
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#include "trx/session/record/UndoDeleteRecordPart.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

#include "bc_base/BinaryUtils.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "schema_table/table_store/TableStore.h"
namespace codablecash {

UndoDeleteRecordPart::UndoDeleteRecordPart(const UndoDeleteRecordPart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_DELETE_RECORD) {
	this->record = inst.record != nullptr ? dynamic_cast<CdbRecord*>(inst.record->copyData()) : nullptr;
}

UndoDeleteRecordPart::UndoDeleteRecordPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_DELETE_RECORD) {
	this->record = nullptr;
}

UndoDeleteRecordPart::~UndoDeleteRecordPart() {
	delete this->record;
}

int UndoDeleteRecordPart::binarySize() const {
	BinaryUtils::checkNotNull(this->record);

	int total = sizeof(uint8_t);

	total += this->record->binarySize();

	return total;
}

void UndoDeleteRecordPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->record);

	out->put(type);
	this->record->toBinary(out);
}

void UndoDeleteRecordPart::fromBinary(ByteBuffer *in) {
	static CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(in); __STP(obj);
	this->record = dynamic_cast<CdbRecord*>(obj);
	BinaryUtils::checkNotNull(this->record);

	__STP_MV(obj);
}

IBlockObject* UndoDeleteRecordPart::copyData() const noexcept {
	return new UndoDeleteRecordPart(*this);
}

void UndoDeleteRecordPart::setRecord(const CdbRecord *record) noexcept {
	delete this->record;
	this->record = dynamic_cast<CdbRecord*>(record->copyData());
}

void UndoDeleteRecordPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	store->insert(this->record);
}

} /* namespace codablecash */
