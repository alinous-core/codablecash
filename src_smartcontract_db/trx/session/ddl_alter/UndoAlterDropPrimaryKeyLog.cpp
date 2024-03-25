/*
 * UndoAlterDropPrimaryKeyLog.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterDropPrimaryKeyLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "bc_base/BinaryUtils.h"

#include "engine/CdbBinaryObject.h"

#include "base/StackRelease.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "trx/session/base/CdbDatabaseSessionId.h"
#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"
#include "trx/session/repo/UndoLogPartsRepo.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbOid.h"


namespace codablecash {

UndoAlterDropPrimaryKeyLog::UndoAlterDropPrimaryKeyLog(const UndoAlterDropPrimaryKeyLog &inst)
		: AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_PRIMARY_KEY) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
}

UndoAlterDropPrimaryKeyLog::UndoAlterDropPrimaryKeyLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_DROP_PRIMARY_KEY) {
	this->table = nullptr;
}

UndoAlterDropPrimaryKeyLog::~UndoAlterDropPrimaryKeyLog() {
	delete this->table;
}

int UndoAlterDropPrimaryKeyLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();

	return total;
}

void UndoAlterDropPrimaryKeyLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(this->type);
	this->table->toBinary(out);
}

void UndoAlterDropPrimaryKeyLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}
}

IBlockObject* UndoAlterDropPrimaryKeyLog::copyData() const noexcept {
	return new UndoAlterDropPrimaryKeyLog(*this);
}

void UndoAlterDropPrimaryKeyLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	handleParts(trxManager, partsRepo, key);
}

void UndoAlterDropPrimaryKeyLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	TableStore* store = storageManager->getTableStore(this->table->getOid());

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		part->execute(trxManager, store);
	}
}

void UndoAlterDropPrimaryKeyLog::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

} /* namespace codablecash */
