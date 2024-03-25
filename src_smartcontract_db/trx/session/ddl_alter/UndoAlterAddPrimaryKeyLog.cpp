/*
 * UndoAlterAddPrimaryKeyLog.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterAddPrimaryKeyLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/TableObjectFactory.h"

#include "bc_base/BinaryUtils.h"

#include "engine/CdbBinaryObject.h"

#include "base/StackRelease.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "engine/CdbOid.h"

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

UndoAlterAddPrimaryKeyLog::UndoAlterAddPrimaryKeyLog(const UndoAlterAddPrimaryKeyLog &inst)
		: AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_PRIMARY_KEY) {
	this->table = inst.table != nullptr ? new CdbTable(*inst.table) : nullptr;
}

UndoAlterAddPrimaryKeyLog::UndoAlterAddPrimaryKeyLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_ADD_PRIMARY_KEY) {
	this->table = nullptr;
}

UndoAlterAddPrimaryKeyLog::~UndoAlterAddPrimaryKeyLog() {
	delete this->table;
}

int UndoAlterAddPrimaryKeyLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();

	return total;
}

void UndoAlterAddPrimaryKeyLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(this->type);
	this->table->toBinary(out);
}

void UndoAlterAddPrimaryKeyLog::fromBinary(ByteBuffer *in) {
	{
		CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTable::CDB_OBJ_TYPE); __STP(obj);
		this->table = dynamic_cast<CdbTable*>(obj);

		this->table->fromBinary(in);

		BinaryUtils::checkNotNull(this->table);
		__STP_MV(obj);
	}
}

IBlockObject* UndoAlterAddPrimaryKeyLog::copyData() const noexcept {
	return new UndoAlterAddPrimaryKeyLog(*this);
}

void UndoAlterAddPrimaryKeyLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	handleParts(trxManager, partsRepo, key);
}

void UndoAlterAddPrimaryKeyLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
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

void UndoAlterAddPrimaryKeyLog::setTable(const CdbTable *tbl) noexcept {
	delete this->table;
	this->table = new CdbTable(*tbl);
}

} /* namespace codablecash */
