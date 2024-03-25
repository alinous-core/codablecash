/*
 * UndoUpdateRecordColumnPart.cpp
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#include "trx/session/record/UpdatedColumn.h"

#include "engine/CdbOid.h"

#include "bc_base/BinaryUtils.h"

#include "schema_table/table_store/TableStore.h"

#include "schema_table/table_store/RecordStore.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "trx/session/record/UndoModifyRecordColumnsPart.h"
namespace codablecash {

UndoModifyRecordColumnsPart::UndoModifyRecordColumnsPart(const UndoModifyRecordColumnsPart &inst)
		: AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_UPDATE_RECORD_COLUMNS) {
	this->recordOid = inst.recordOid != nullptr ? inst.recordOid->copy() : nullptr;
	this->list = new ArrayList<UpdatedColumn>();
}

UndoModifyRecordColumnsPart::UndoModifyRecordColumnsPart()
		: AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_UPDATE_RECORD_COLUMNS) {
	this->recordOid = nullptr;
	this->list = new ArrayList<UpdatedColumn>();
}

UndoModifyRecordColumnsPart::~UndoModifyRecordColumnsPart() {
	delete this->recordOid;

	this->list->deleteElements();
	delete this->list;
}

int UndoModifyRecordColumnsPart::binarySize() const {
	BinaryUtils::checkNotNull(this->recordOid);

	int total = sizeof(uint8_t);

	total += this->recordOid->binarySize();

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		UpdatedColumn* ucol = this->list->get(i);

		total += ucol->binarySize();
	}

	return total;
}

void UndoModifyRecordColumnsPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->recordOid);

	out->put(type);
	this->recordOid->toBinary(out);

	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		UpdatedColumn* ucol = this->list->get(i);
		ucol->toBinary(out);
	}
}

void UndoModifyRecordColumnsPart::fromBinary(ByteBuffer *in) {
	this->recordOid = CdbOid::fromBinary(in);
	BinaryUtils::checkNotNull(this->recordOid);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		UpdatedColumn* ucol = UpdatedColumn::fromBinary(in);
		BinaryUtils::checkNotNull(ucol);

		this->list->addElement(ucol);
	}
}

IBlockObject* UndoModifyRecordColumnsPart::copyData() const noexcept {
	return new UndoModifyRecordColumnsPart(*this);
}

void UndoModifyRecordColumnsPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	RecordStore* recordStore = store->getRecordStore();

	CdbRecord* record = recordStore->findRecord(this->recordOid); __STP(record);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		UpdatedColumn* ucol = this->list->get(i);

		ucol->update(record);
	}

	recordStore->insert(record);
}

void UndoModifyRecordColumnsPart::setRecordOid(const CdbOid *oid) noexcept {
	delete this->recordOid;
	this->recordOid = oid->copy();
}

void UndoModifyRecordColumnsPart::addUpdatedColumn(int pos, const AbstractCdbValue *lastValue) noexcept {
	UpdatedColumn* col = new UpdatedColumn(pos, lastValue);
	this->list->addElement(col);
}

} /* namespace codablecash */
