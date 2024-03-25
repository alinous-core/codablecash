/*
 * UndoAddIndexPart.cpp
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#include "trx/session/index/UndoAddIndexPart.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/table_store/TableStore.h"

#include "schema_table/schema/SchemaManager.h"

#include "trx/transaction/CdbTransactionManager.h"
namespace codablecash {

UndoAddIndexPart::UndoAddIndexPart(const UndoAddIndexPart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_ADD_INDEX) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->indexOid = inst.indexOid;
}

UndoAddIndexPart::UndoAddIndexPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_ADD_INDEX) {
	this->table = nullptr;
	this->indexOid = 0L;
}

UndoAddIndexPart::~UndoAddIndexPart() {
	delete this->table;
}

int UndoAddIndexPart::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += sizeof(this->indexOid);

	return total;
}

void UndoAddIndexPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(type);

	this->table->toBinary(out);
	out->putLong(this->indexOid);
}

void UndoAddIndexPart::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	this->indexOid = in->getLong();
}

IBlockObject* UndoAddIndexPart::copyData() const noexcept {
	return new UndoAddIndexPart(*this);
}

void UndoAddIndexPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	CdbOid tableOid(this->table->getOid()->getOidValue());
	CdbOid indexOid(this->indexOid);

	CdbTable* table = schemaManager->getTable(&tableOid);

	CdbTableIndex* removalIndex = table->getIndex(&indexOid); __STP(removalIndex);
	assert(removalIndex != nullptr);

	table->removeIndex(removalIndex);

	// remove data
	store->removeIndex(removalIndex);

	schemaManager->save();
}

void UndoAddIndexPart::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

void UndoAddIndexPart::setIndexOid(uint64_t oid) noexcept {
	this->indexOid = oid;
}

} /* namespace codablecash */
