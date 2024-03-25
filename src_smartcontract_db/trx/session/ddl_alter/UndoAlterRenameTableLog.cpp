/*
 * UndoAlterRenameTableLog.cpp
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */

#include "trx/session/ddl_alter/UndoAlterRenameTableLog.h"

#include "base/UnicodeString.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "schema_table/schema_alter_ctx/TableRenameContext.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/schema/SchemaManager.h"

#include "bc_base/BinaryUtils.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsIdkey.h"

#include "engine/CdbOid.h"

#include "trx/session/repo/UndoLogPartsScanner.h"

#include "base/StackRelease.h"

#include "trx/session/base/AbstractUndoLogPart.h"
namespace codablecash {

UndoAlterRenameTableLog::UndoAlterRenameTableLog(const UndoAlterRenameTableLog &inst)
		: AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_RENAME_TABLE) {
	this->srcSchema = inst.srcSchema != nullptr ? new UnicodeString(*inst.srcSchema) : nullptr;
	this->dstSchema = inst.dstSchema != nullptr ? new UnicodeString(*inst.dstSchema) : nullptr;

	this->srcTable = inst.srcTable != nullptr ? new UnicodeString(*inst.srcTable) : nullptr;
	this->dstTable = inst.dstTable != nullptr ? new UnicodeString(*inst.dstTable) : nullptr;
}

UndoAlterRenameTableLog::UndoAlterRenameTableLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_ALTER_RENAME_TABLE) {
	this->srcSchema = nullptr;
	this->dstSchema = nullptr;

	this->srcTable = nullptr;
	this->dstTable = nullptr;
}

UndoAlterRenameTableLog::~UndoAlterRenameTableLog() {
	delete this->srcSchema;
	delete this->dstSchema;

	delete this->srcTable;
	delete this->dstTable;
}

int UndoAlterRenameTableLog::binarySize() const {
	BinaryUtils::checkNotNull(this->dstSchema);
	BinaryUtils::checkNotNull(this->srcSchema);
	BinaryUtils::checkNotNull(this->dstTable);
	BinaryUtils::checkNotNull(this->srcTable);

	int total = sizeof(uint8_t);

	total += BinaryUtils::stringSize(this->dstSchema);
	total += BinaryUtils::stringSize(this->srcSchema);
	total += BinaryUtils::stringSize(this->dstTable);
	total += BinaryUtils::stringSize(this->srcTable);

	return total;
}

void UndoAlterRenameTableLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->dstSchema);
	BinaryUtils::checkNotNull(this->srcSchema);
	BinaryUtils::checkNotNull(this->dstTable);
	BinaryUtils::checkNotNull(this->srcTable);

	out->put(this->type);

	BinaryUtils::putString(out, this->dstSchema);
	BinaryUtils::putString(out, this->srcSchema);
	BinaryUtils::putString(out, this->dstTable);
	BinaryUtils::putString(out, this->srcTable);
}

void UndoAlterRenameTableLog::fromBinary(ByteBuffer *in) {
	this->dstSchema = BinaryUtils::getString(in);
	this->srcSchema = BinaryUtils::getString(in);
	this->dstTable = BinaryUtils::getString(in);
	this->srcTable = BinaryUtils::getString(in);


	BinaryUtils::checkNotNull(this->dstSchema);
	BinaryUtils::checkNotNull(this->srcSchema);
	BinaryUtils::checkNotNull(this->dstTable);
	BinaryUtils::checkNotNull(this->srcTable);
}

IBlockObject* UndoAlterRenameTableLog::copyData() const noexcept {
	return new UndoAlterRenameTableLog(*this);
}

void UndoAlterRenameTableLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	SchemaManager* schemaManamger = trxManager->getSchemaManager();

	TableRenameContext context;

	context.init(this->dstSchema, this->dstTable, this->srcSchema, this->srcTable, schemaManamger);
	context.validate(schemaManamger);

	context.commitSchemaDir(schemaManamger);
	context.commit(schemaManamger);

	schemaManamger->save();

	handleParts(trxManager, partsRepo, key);
}

void UndoAlterRenameTableLog::handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey *key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		part->execute(trxManager, nullptr);
	}
}

void UndoAlterRenameTableLog::importFromContext(const TableRenameContext *context) noexcept {
	this->srcSchema = new UnicodeString(context->getSrcSchema());
	this->dstSchema = new UnicodeString(context->getDstSchema());

	this->srcTable = new UnicodeString(context->getTable()->getName());
	this->dstTable = new UnicodeString(context->getDstTable());
}

} /* namespace codablecash */
