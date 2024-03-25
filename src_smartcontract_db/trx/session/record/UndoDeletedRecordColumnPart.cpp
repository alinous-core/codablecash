/*
 * UndoDeletedRecordColumn.cpp
 *
 *  Created on: 2023/08/01
 *      Author: iizuka
 */
#include "trx/session/record/UndoDeletedRecordColumnPart.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/table_store/TableStore.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

#include "engine/CdbOid.h"

#include "schema_table/table_store/RecordStore.h"

namespace codablecash {

UndoDeletedRecordColumnPart::UndoDeletedRecordColumnPart(const UndoDeletedRecordColumnPart &inst)
		: AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_DELETED_RECORD_COLUMN) {
	this->recordOid = inst.recordOid;
	this->pos = inst.pos;
	this->lastValue = inst.lastValue != nullptr ? inst.lastValue->copy() : nullptr;
}

UndoDeletedRecordColumnPart::UndoDeletedRecordColumnPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_DELETED_RECORD_COLUMN) {
	this->recordOid = 0L;
	this->pos = 0L;
	this->lastValue = 0;
}

UndoDeletedRecordColumnPart::~UndoDeletedRecordColumnPart() {
	delete this->lastValue;
}

int UndoDeletedRecordColumnPart::binarySize() const {
	int total = sizeof(uint8_t);

	total += sizeof(this->recordOid);
	total += sizeof(this->pos);

	bool notnull = this->lastValue != nullptr;
	total += sizeof(uint8_t);

	if(notnull){
		total += this->lastValue->binarySize();
	}

	return total;
}

void UndoDeletedRecordColumnPart::toBinary(ByteBuffer *out) const {
	out->put(type);

	out->putLong(this->recordOid);
	out->putShort(this->pos);

	bool notnull = this->lastValue != nullptr;
	out->put(notnull ? 1 : 0);

	if(notnull){
		this->lastValue->toBinary(out);
	}
}

void UndoDeletedRecordColumnPart::fromBinary(ByteBuffer *in) {
	this->recordOid = in->getLong();
	this->pos = in->getShort();

	uint8_t bl = in->get();
	if(bl > 0){
		static CdbDataFactory factory;
		IBlockObject* obj = factory.makeDataFromBinary(in); __STP(obj);
		this->lastValue = dynamic_cast<AbstractCdbValue*>(obj);

		BinaryUtils::checkNotNull(lastValue);
		__STP_MV(obj);
	}
}

IBlockObject* UndoDeletedRecordColumnPart::copyData() const noexcept {
	return new UndoDeletedRecordColumnPart(*this);
}

void UndoDeletedRecordColumnPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	CdbOid recordOid(this->recordOid);

	RecordStore* recordStore = store->getRecordStore();

	CdbRecord* record = store->findRecord(&recordOid); __STP(record);

	AbstractCdbValue* value = this->lastValue != nullptr ? this->lastValue->copy() : nullptr;
	record->insertValue(value, this->pos);

	// do not touch index
	recordStore->remove(&recordOid);
	recordStore->insert(record);
}

void UndoDeletedRecordColumnPart::setRecordOid(uint64_t oid) noexcept {
	this->recordOid = oid;
}

void UndoDeletedRecordColumnPart::setPosition(int position) noexcept {
	this->pos = position;
}

void UndoDeletedRecordColumnPart::setLastValue(const AbstractCdbValue *value) noexcept {
	delete this->lastValue;
	this->lastValue = value != nullptr ? value->copy() : nullptr;
}

} /* namespace codablecash */
