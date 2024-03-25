/*
 * CreateTableLog.cpp
 *
 *  Created on: 2020/05/12
 *      Author: iizuka
 */

#include "trx/transaction_log/CreateTableLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "trx/transaction/SchemaObjectIdPublisher.h"
#include "trx/transaction/CdbTransactionManager.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "engine_lock/WriteLockHandle.h"
#include "engine_lock/StackDbLockUnlocker.h"

namespace codablecash {

CreateTableLog::CreateTableLog(uint64_t oid) : AbstractDdlLog(AbstractTransactionLog::TRX_CREATE_TABLE, oid) {
	this->table = nullptr;
}

CreateTableLog::~CreateTableLog() {
	delete this->table;
}

int CreateTableLog::binarySize() const {
	checkNotNull(this->table);
	int total = sizeof(uint8_t) + sizeof(uint64_t);

	total += this->table->binarySize();

	return total;
}

void CreateTableLog::toBinary(ByteBuffer* out) const {
	checkNotNull(this->table);

	out->put(AbstractTransactionLog::TRX_CREATE_TABLE);
	out->putLong(this->oid);

	this->table->toBinary(out);
}

void CreateTableLog::fromBinary(ByteBuffer* in) {
	CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE);

	this->table = dynamic_cast<CdbTable*>(obj);
	this->table->fromBinary(in);
}

void CreateTableLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	WriteLockHandle* lockH = trxManager->databaseWriteLock();
	StackDbLockUnlocker unlocker(lockH);

	this->table->setupUniqueIndexes();

	SchemaObjectIdPublisher* publisher= trxManager->getSchemaObjectIdPublisher();
	this->table->assignNewOid(publisher);

	trxManager->commitCreateTable(this);
}


void CreateTableLog::setTable(CdbTable* table) noexcept {
	this->table = table;
}

} /* namespace codablecash */
