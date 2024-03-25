/*
 * UndoIndexPrimaryPart.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#include "trx/session/index/UndoIndexPrimaryPart.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/schema/SchemaManager.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbOid.h"

namespace codablecash {

UndoIndexPrimaryPart::UndoIndexPrimaryPart(const UndoIndexPrimaryPart &inst)
		: AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INDEX_PRIMARY) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->indexOid = inst.indexOid;
	this->lastPrimary = inst.lastPrimary;
}

UndoIndexPrimaryPart::UndoIndexPrimaryPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INDEX_PRIMARY) {
	this->table = nullptr;
	this->indexOid = 0L;
	this->lastPrimary = false;
}

UndoIndexPrimaryPart::~UndoIndexPrimaryPart() {
	delete this->table;
}

int UndoIndexPrimaryPart::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += sizeof(this->indexOid);
	total += sizeof(uint8_t);

	return total;
}

void UndoIndexPrimaryPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(type);

	this->table->toBinary(out);
	out->putLong(this->indexOid);
	out->put(this->lastPrimary ? 1 : 0);
}

void UndoIndexPrimaryPart::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	this->indexOid = in->getLong();

	uint8_t bl = in->get();
	this->lastPrimary = bl > 0;
}

IBlockObject* UndoIndexPrimaryPart::copyData() const noexcept {
	return new UndoIndexPrimaryPart(*this);
}

void UndoIndexPrimaryPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	SchemaManager *schemaManager = trxManager->getSchemaManager();

	CdbOid tableOid(this->table->getOid()->getOidValue());
	CdbOid indexOid(this->indexOid);

	CdbTable* table = schemaManager->getTable(&tableOid);

	CdbTableIndex* updatedIndex = table->getIndex(&indexOid);
	assert(updatedIndex != nullptr);

	updatedIndex->setPrimaryKey(this->lastPrimary);

	schemaManager->save();
}

void UndoIndexPrimaryPart::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

void UndoIndexPrimaryPart::setIndexOid(uint64_t oid) noexcept {
	this->indexOid = oid;
}

void UndoIndexPrimaryPart::setLastPrimary(bool bl) noexcept {
	this->lastPrimary = bl;
}

} /* namespace codablecash */
