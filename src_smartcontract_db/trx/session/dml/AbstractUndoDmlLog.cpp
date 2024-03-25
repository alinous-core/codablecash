/*
 * AbstractUndoDmlLog.cpp
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#include "trx/session/dml/AbstractUndoDmlLog.h"

#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsScanner.h"

#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"

#include "engine/CdbOid.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"


namespace codablecash {

AbstractUndoDmlLog::AbstractUndoDmlLog(const AbstractUndoDmlLog &inst) : AbstractUndoLog(inst.type) {
	this->table = inst.table != nullptr ? new CdbTableIdentifier(*inst.table) : nullptr;
}

AbstractUndoDmlLog::AbstractUndoDmlLog(uint8_t type) : AbstractUndoLog(type) {
	this->table = nullptr;
}

AbstractUndoDmlLog::~AbstractUndoDmlLog() {
	delete this->table;
}

int AbstractUndoDmlLog::binarySize() const {
	BinaryUtils::checkNotNull(this->table);

	int total = sizeof(uint8_t);

	total += this->table->binarySize();

	return total;
}

void AbstractUndoDmlLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->table);

	out->put(this->type);
	this->table->toBinary(out);
}

void AbstractUndoDmlLog::fromBinary(ByteBuffer *in) {
	this->table = CdbTableIdentifier::fromBinary(in);
	BinaryUtils::checkNotNull(this->table);
}

void AbstractUndoDmlLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	handleParts(trxManager, partsRepo, key);
}

void AbstractUndoDmlLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	CdbTable* table = schemaManager->getTable(this->table->getSchema(), this->table->getTable());

	TableStore* store = storageManager->getTableStore(table->getOid());

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		part->execute(trxManager, store);
	}
}

void AbstractUndoDmlLog::setTableId(const CdbTableIdentifier *table) noexcept {
	delete this->table;
	this->table = new CdbTableIdentifier(*table);
}

} /* namespace codablecash */
