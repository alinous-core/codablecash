/*
 * UndoIndexUniquePart.cpp
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#include "trx/session/index/UndoIndexUniquePart.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/schema/SchemaManager.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "trx/transaction/CdbTransactionManager.h"

namespace codablecash {

UndoIndexUniquePart::UndoIndexUniquePart(const UndoIndexUniquePart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INDEX_UNIQUE) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->indexOid = inst.indexOid;
	this->lastUnique = inst.lastUnique;
}

UndoIndexUniquePart::UndoIndexUniquePart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_INDEX_UNIQUE) {
	this->table = nullptr;
	this->indexOid = 0L;
	this->lastUnique = false;
}

UndoIndexUniquePart::~UndoIndexUniquePart() {
	delete this->table;
}

int UndoIndexUniquePart::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += sizeof(this->indexOid);
	total += sizeof(uint8_t);

	return total;
}

void UndoIndexUniquePart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(type);

	this->table->toBinary(out);
	out->putLong(this->indexOid);
	out->put(this->lastUnique ? 1 : 0);
}

void UndoIndexUniquePart::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	this->indexOid = in->getLong();

	uint8_t bl = in->get();
	this->lastUnique = bl > 0;
}

IBlockObject* UndoIndexUniquePart::copyData() const noexcept {
	return new UndoIndexUniquePart(*this);
}

void UndoIndexUniquePart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	SchemaManager *schemaManager = trxManager->getSchemaManager();

	CdbOid tableOid(this->table->getOid()->getOidValue());
	CdbOid indexOid(this->indexOid);

	CdbTable* table = schemaManager->getTable(&tableOid);

	CdbTableIndex* updatedIndex = table->getIndex(&indexOid);
	assert(updatedIndex != nullptr);

	updatedIndex->setUnique(this->lastUnique);

	schemaManager->save();
}

void UndoIndexUniquePart::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

void UndoIndexUniquePart::setIndexOid(uint64_t oid) noexcept {
	this->indexOid = oid;
}

void UndoIndexUniquePart::setLastUnique(bool bl) noexcept {
	this->lastUnique = bl;
}

} /* namespace codablecash */
