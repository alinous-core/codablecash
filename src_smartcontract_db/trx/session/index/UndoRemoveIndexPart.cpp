/*
 * UndoRemoveIndexPart.cpp
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#include "trx/session/index/UndoRemoveIndexPart.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/TableObjectFactory.h"

#include "schema_table/schema/SchemaManager.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"


namespace codablecash {

UndoRemoveIndexPart::UndoRemoveIndexPart(const UndoRemoveIndexPart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_REMOVE_INDEX) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
	this->removedindex = inst.removedindex != nullptr ? new CdbTableIndex(*inst.removedindex) : nullptr;
}

UndoRemoveIndexPart::UndoRemoveIndexPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_REMOVE_INDEX) {
	this->table = nullptr;
	this->removedindex = nullptr;
}

UndoRemoveIndexPart::~UndoRemoveIndexPart() {
	delete this->table;
	delete this->removedindex;
}

int UndoRemoveIndexPart::binarySize() const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->removedindex);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();
	total += this->removedindex->binarySize();

	return total;
}

void UndoRemoveIndexPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);
	BinaryUtils::checkNotNull(this->removedindex);

	out->put(type);

	this->table->toBinary(out);
	this->removedindex->toBinary(out);
}

void UndoRemoveIndexPart::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}

	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableIndex::CDB_OBJ_TYPE); __STP(obj);
		this->removedindex = dynamic_cast<CdbTableIndex*>(obj);
		this->removedindex->fromBinary(in, this->table);

		BinaryUtils::checkNotNull(this->removedindex);
		__STP_MV(obj);
	}
}

IBlockObject* UndoRemoveIndexPart::copyData() const noexcept {
	return new UndoRemoveIndexPart(*this);
}

void UndoRemoveIndexPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	const UnicodeString* schemaName = this->table->getSchemaName();
	const UnicodeString* tableName = this->table->getName();

	CdbTableIndex *index = new CdbTableIndex(*this->removedindex);

	schemaManager->recoverIndex(trxManager, index, schemaName, tableName);
}

void UndoRemoveIndexPart::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

void UndoRemoveIndexPart::setIndex(const CdbTableIndex *index) noexcept {
	delete this->removedindex;
	this->removedindex = new CdbTableIndex(*index);
}

} /* namespace codablecash */
