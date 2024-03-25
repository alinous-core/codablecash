/*
 * SchemaAlterCommandsHandler.cpp
 *
 *  Created on: 2020/10/05
 *      Author: iizuka
 */

#include "schema_table/schema/SchemaAlterCommandsHandler.h"

#include "engine/CdbException.h"
#include "engine/CdbOid.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "trx/transaction_log_alter/AlterAddIndexCommandLog.h"
#include "trx/transaction_log_alter/AlterAddColumnCommandLog.h"
#include "trx/transaction_log_alter/AlterDropIndexCommandLog.h"
#include "trx/transaction_log_alter/AlterDropColumnCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterAddPrimaryKeyCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterDropPrimaryKeyCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterModifyCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterRenameColumnCommandLog.h"
#include "trx/transaction_log_alter_modify/AlterRenameTableCommandLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/schema/SchemaManager.h"
#include "schema_table/schema/SchemaRoot.h"

#include "schema_table/schema_alter_ctx/ColumnModifyContext.h"
#include "schema_table/schema_alter_ctx/TableRenameContext.h"

#include "lang_sql/sql_ddl/DdlColumnDescriptor.h"

#include "lang_sql/sql_join_parts/TableIdentifier.h"

#include "lang_sql/sql_ddl_alter_modify/AlterAddPrimaryKeyCommand.h"
#include "lang_sql/sql_ddl_alter_modify/AlterRenameColumnCommand.h"
#include "lang_sql/sql_ddl_alter_modify/AlterModifyCommand.h"

#include "lang_sql/sql_ddl_alter/AlterAddIndexCommand.h"
#include "lang_sql/sql_ddl_alter/AlterDropIndexCommand.h"
#include "lang_sql/sql_ddl_alter/AlterAddColumnCommand.h"
#include "lang_sql/sql_ddl_alter/AlterDropColumnCommand.h"

#include "lang_sql/sql_ddl/ColumnTypeDescriptor.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "trx/session/base/UndoSpaceManager.h"
#include "trx/session/base/CdbDatabaseSession.h"
#include "trx/session/base/UndoRegisterTransaction.h"

#include "trx/session/ddl_alter/UndoAlterAddIndexLog.h"
#include "trx/session/ddl_alter/UndoAlterDropIndexLog.h"
#include "trx/session/ddl_alter/UndoAlterModifyLog.h"
#include "trx/session/ddl_alter/UndoAlterAddColumnLog.h"
#include "trx/session/ddl_alter/UndoAlterDropColumnLog.h"
#include "trx/session/ddl_alter/UndoAlterDropPrimaryKeyLog.h"
#include "trx/session/ddl_alter/UndoAlterAddPrimaryKeyLog.h"
#include "trx/session/ddl_alter/UndoAlterRenameTableLog.h"
#include "trx/session/ddl_alter/UndoAlterRenameColumnLog.h"

#include "trx/session/index/UndoIndexUniquePart.h"
#include "trx/session/index/UndoRemoveIndexPart.h"
#include "trx/session/index/UndoAddIndexPart.h"
#include "trx/session/index/UndoIndexPrimaryPart.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"

#include "trx/session/record/UndoCreateSchemaPart.h"


using namespace alinous;

namespace codablecash {

SchemaAlterCommandsHandler::SchemaAlterCommandsHandler(SchemaManager* schemaManager) {
	this->schemaManager = schemaManager;
}

SchemaAlterCommandsHandler::~SchemaAlterCommandsHandler() {
	this->schemaManager = nullptr;
}

void SchemaAlterCommandsHandler::handleAlterTableAddIndex(const AlterAddIndexCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	const AlterAddIndexCommand* command = cmd->getCommand();

	CdbTable* table = findTableFromCommand(cmd);

	uint64_t newOid = this->schemaManager->root->newSchemaObjectId();
	CdbTableIndex* newIndex = new CdbTableIndex(newOid);

	const UnicodeString* indexName = command->getName();
	newIndex->setName(new UnicodeString(indexName));

	const ArrayList<UnicodeString>* colList = command->getList();
	int maxLoop = colList->size();
	for(int i = 0; i != maxLoop; ++i){
		const UnicodeString* colstr = colList->get(i);

		const CdbTableColumn* col = table->getColumn(colstr);
		assert(col != nullptr);

		newIndex->addColumn(col->getOid());
	}

	newIndex->setUnique(command->isUnique());
	table->addIndex(newIndex);

	this->schemaManager->fireOnAddIndex(trxManager, table, newIndex);

	// undo
	{
		UndoAlterAddIndexLog log;
		log.setNewIndexOid(newOid);

		const CdbOid* tableOid = table->getOid();
		log.setTableOid(tableOid->getOidValue());

		session->addUndoLog(&log);
	}
	undoTrx.logAdded();

	// save
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableDropIndex(const AlterDropIndexCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	const AlterDropIndexCommand* command = cmd->getCommand();

	CdbTable* table = findTableFromCommand(cmd);

	const UnicodeString* indexName = command->getName();
	CdbTableIndex* removalIndex = table->getIndexByName(indexName); __STP(removalIndex);
	assert(removalIndex != nullptr);

	{
		UndoAlterDropIndexLog log;

		const CdbOid* tableOid = table->getOid();
		log.setTable(table);
		log.setTableIndex(removalIndex);

		session->addUndoLog(&log);
	}
	undoTrx.logAdded();

	table->removeIndex(removalIndex);

	this->schemaManager->fireOnDropIndex(trxManager, table, removalIndex);

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableAddColumn(const AlterAddColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	AlterAddColumnCommand* command = cmd->getCommand();

	CdbTable* table = findTableFromCommand(cmd);
	CdbTable* lastTable = new CdbTable(*table); __STP(lastTable);

	const DdlColumnDescriptor* columnDesc = command->getColumnDescriptor();
	const ColumnTypeDescriptor* typeDesc = columnDesc->getTypeDesc();

	uint64_t newOid = this->schemaManager->root->newSchemaObjectId();
	const UnicodeString* colName = columnDesc->getName();
	uint8_t cdbType = typeDesc->toCdbValueType();
	int colLength = command->getLengthValue();
	bool notnull = columnDesc->isNotNull();
	bool unique = columnDesc->isUnique();
	const UnicodeString* defValue = command->getDefaultValueStr();

	// undo log
	{
		UndoAlterAddColumnLog log;
		log.setTable(lastTable);
		log.setnewOid(newOid);

		session->addUndoLog(&log);
	}

	CdbOid oidObj(newOid);
	table->addColumn(&oidObj, colName, cdbType, colLength, notnull, unique, defValue);

	CdbTableColumn* newColumn = table->getColumn(colName);
	CdbTableIndex* newUniqueIndex = nullptr;
	if(newColumn->isUnique()){
		uint64_t newIdxOid = this->schemaManager->root->newSchemaObjectId();
		newUniqueIndex = new CdbTableIndex(newIdxOid);

		UnicodeString* indexName = CdbTableIndex::createUniqueKeyIndexName(table, newColumn->getName());
		newUniqueIndex->setName(indexName);
		newUniqueIndex->addColumn(newColumn->getOid());
		newUniqueIndex->setUnique(true);
		newUniqueIndex->setPrimaryKey(false);

		table->addIndex(newUniqueIndex);

		// undo log part
		{
			UndoAddIndexPart logPart;
			logPart.setTable(lastTable);
			logPart.setIndexOid(newIdxOid);
			session->addUndoLogPart(&logPart);
		}
	}

	this->schemaManager->fireOnAddColumn(trxManager, table, newColumn, newUniqueIndex);

	// save
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableDropColumn(const AlterDropColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	const AlterDropColumnCommand* command = cmd->getCommand();

	CdbTable* table = findTableFromCommand(cmd);

	const UnicodeString* columnName = command->getName();

	// undo log
	{
		CdbTableColumn* col = table->getColumn(columnName);

		UndoAlterDropColumnLog log;
		log.setTable(table);
		log.setRemoveColumn(col);

		session->addUndoLog(&log);

		CdbStorageManager* storageManager = trxManager->getStorageManager();
		TableStore* store = storageManager->getTableStore(table->getOid());

		int pos = col->getPosition();
		store->addUndoDeleteColumnPart(session, pos);
	}

	ArrayList<CdbTableIndex>* removalIndexes = table->removeIndexesUsingColumn(columnName); __STP(removalIndexes);
	removalIndexes->setDeleteOnExit();

	// undo remove indexParts
	{
		int maxLoop = removalIndexes->size();
		for(int i = 0; i != maxLoop; ++i){
			CdbTableIndex* index = removalIndexes->get(i);

			UndoRemoveIndexPart part;
			part.setTable(table);
			part.setIndex(index);
			session->addUndoLogPart(&part);
		}
	}

	CdbTableColumn* removalColumn = table->removeColumn(columnName); __STP(removalColumn);

	this->schemaManager->fireOnDropColumn(trxManager, table, removalColumn, removalIndexes);

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableAddPrimaryKey(const AlterAddPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	CdbTable* table = findTableFromCommand(cmd);

	const AlterAddPrimaryKeyCommand* command = cmd->getCommand();
	const ArrayList<UnicodeString>* collist = command->getColumns();

	// undo log
	{
		UndoAlterAddPrimaryKeyLog log;
		log.setTable(table);

		session->addUndoLog(&log);
	}

	const CdbTableIndex* newidx = table->setPrimaryKeys(collist);

	if(newidx != nullptr){
		// add undo part
		{
			UndoAddIndexPart partLog;
			partLog.setTable(table);
			partLog.setIndexOid(newidx->getOid()->getOidValue());

			session->addUndoLogPart(&partLog);
		}
		this->schemaManager->fireOnAddPrimaryKey(trxManager, table, newidx);
	}
	else{
		// add undo part
		ArrayList<const CdbOid> oidlist;

		int maxLoop = collist->size();
		for(int i = 0; i != maxLoop; ++i){
			const UnicodeString* c = collist->get(i);
			CdbTableColumn* col = table->getColumn(c);

			const CdbOid* oid = col->getOid();
			oidlist.addElement(oid);
		}
		CdbTableIndex* prevIndex = table->getIndexByColumnOidsStrict(&oidlist, true);

		UndoIndexPrimaryPart partLog;
		partLog.setTable(table);
		partLog.setIndexOid(prevIndex->getOid()->getOidValue());
		partLog.setLastPrimary(false);

		session->addUndoLogPart(&partLog);
	}

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableDropPrimaryKey(const AlterDropPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	CdbTable* table = findTableFromCommand(cmd);

	CdbTableIndex* primaryKey = table->getPrimaryKey();
	StackRelease<CdbTableIndex> stPrimaryKey(primaryKey);

	// undo log
	{
		UndoAlterDropPrimaryKeyLog log;
		log.setTable(table);

		session->addUndoLog(&log);
	}

	assert(primaryKey != nullptr);

	// check unique of column on length == 1
	if(primaryKey->getColumnLength() == 1){
		const CdbOid* colOid = primaryKey->getColumnAt(0);
		CdbTableColumn* col = table->getColumn(colOid);

		if(col->isUnique()){
			primaryKey->setPrimaryKey(false);
			stPrimaryKey.cancel();

			{
				UndoIndexPrimaryPart partLog;
				partLog.setTable(table);
				partLog.setIndexOid(primaryKey->getOid()->getOidValue());
				partLog.setLastPrimary(true);

				session->addUndoLogPart(&partLog);
			}

			// upgrade
			this->schemaManager->root->upgradeSchemaObjectVersionId();
			this->schemaManager->save();
			return;
		}
	}

	// log part
	{
		UndoRemoveIndexPart partLog;
		partLog.setTable(table);
		partLog.setIndex(primaryKey);

		session->addUndoLogPart(&partLog);
	}
	table->removeIndex(primaryKey);

	this->schemaManager->fireOnDropPrimaryKey(trxManager, table, primaryKey);

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableModify(const AlterModifyCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);


	CdbTable* table = this->schemaManager->findTableFromCommand(cmd);

	const AlterModifyCommand* modifyCommand = cmd->getCommand();

	const DdlColumnDescriptor* newdesc = modifyCommand->getColumnDescriptor();

	CdbTableColumn* col = table->getColumn(newdesc->getName());
	if(col == nullptr){
		throw new CdbException(L"Column does not exists.", __FILE__, __LINE__);
	}

	CdbTableColumn* originalCol = new CdbTableColumn(*col); __STP(originalCol);

	const UnicodeString* defaultStr = cmd->getCommand()->getDefaultValueStr();

	ColumnModifyContext* context = col->createModifyContextwithChange(modifyCommand, defaultStr); __STP(context);
	context->setColumn(col);

	// alter modify undo
	{
		UndoAlterModifyLog log;
		log.setTableOid(table->getOid()->getOidValue());
		log.setColumn(originalCol);

		// set if data is modified in store class
		ColumnModifyContext::NotNullChage notNullChange = context->getNotNullChange();
		log.setDataModified(context->isTypeChanged() || notNullChange == ColumnModifyContext::NotNullChage::TO_NOT_NULL);

		session->addUndoLog(&log);
	}
	undoTrx.logAdded();

	// convert default
	context->analyze();

	handleUniqueIndexOnModify(table, context, trxManager);

	// update storage
	this->schemaManager->fireOnAlterModify(trxManager, table, context);

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleUniqueIndexOnModify(CdbTable* table,	ColumnModifyContext* ctx, CdbTransactionManager* trxManager) {
	ColumnModifyContext::UniqueChage change = ctx->getUniqueChange();

	if(change == ColumnModifyContext::UniqueChage::TO_NOT_UNIQUE){
		handleToNotUnique(table, ctx, trxManager);
	}
	else if(change == ColumnModifyContext::UniqueChage::TO_UNIQUE){
		handleToUnique(table, ctx, trxManager);
	}
}

void SchemaAlterCommandsHandler::handleToNotUnique(CdbTable* table, ColumnModifyContext* ctx, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();

	CdbTableColumn* col = ctx->getColumn();
	const CdbOid* colOid = col->getOid();

	ArrayList<const CdbOid> oidlist;
	oidlist.addElement(colOid);

	CdbTableIndex* index = table->getIndexByColumnOidsStrict(&oidlist, true);
	if(index == nullptr || index->isPrimaryKey()){ // leave primary key
		if(index != nullptr){
			UndoIndexUniquePart logPart;
			logPart.setTable(table);
			logPart.setIndexOid(index->getOid()->getOidValue());
			logPart.setLastUnique(index->isUnique());
			session->addUndoLogPart(&logPart);

			index->setUnique(false);
		}
		return;
	}

	ctx->setRemovalIndex(index);
	table->removeIndex(index);

	UndoRemoveIndexPart logPart;
	logPart.setTable(table);
	logPart.setIndex(index);

	session->addUndoLogPart(&logPart);
}

void SchemaAlterCommandsHandler::handleToUnique(CdbTable* table, ColumnModifyContext* ctx, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();

	CdbTableColumn* col = ctx->getColumn();
	const CdbOid* colOid = col->getOid();

	// already has primary key
	if(table->hasSinglePrimaryKeyColumn(colOid)){
		CdbTableIndex* idx = table->getPrimaryKey();

		{
			UndoIndexUniquePart logPart;
			logPart.setTable(table);
			logPart.setIndexOid(idx->getOid()->getOidValue());
			logPart.setLastUnique(idx->isUnique());
			session->addUndoLogPart(&logPart);
		}

		idx->setUnique(true);

		return;
	}

	// already has not-unique or unique index
	ArrayList<const CdbOid> oidlist;
	oidlist.addElement(colOid);
	CdbTableIndex* idx = table->getIndexByColumnOidsStrict(&oidlist, false);
	if(idx != nullptr){
		{
			UndoIndexUniquePart logPart;
			logPart.setTable(table);
			logPart.setIndexOid(idx->getOid()->getOidValue());
			logPart.setLastUnique(idx->isUnique());
			session->addUndoLogPart(&logPart);
		}

		idx->setUnique(true);
		return;
	}

	uint64_t newOid = this->schemaManager->root->newSchemaObjectId();
	CdbTableIndex* newIndex = new CdbTableIndex(newOid);
	ctx->setNewIndex(newIndex);

	UnicodeString* indexName = CdbTableIndex::createUniqueKeyIndexName(table, col->getName());
	newIndex->setName(indexName);
	newIndex->addColumn(col->getOid());
	newIndex->setUnique(true);

	table->addIndex(newIndex);

	{
		UndoAddIndexPart logPart;
		logPart.setTable(table);
		logPart.setIndexOid(newOid);
		session->addUndoLogPart(&logPart);
	}
}

void SchemaAlterCommandsHandler::handleAlterTableRenameColumn(const AlterRenameColumnCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	const AlterRenameColumnCommand* command = cmd->getCommand();
	const UnicodeString* lastName = command->getLastName();
	const UnicodeString* newName = command->getNewName();

	CdbTable* table = findTableFromCommand(cmd);
	table->renameColumn(lastName, newName);

	// undo log
	{
		UndoAlterRenameColumnLog log;
		log.setTable(table);
		log.setNewName(newName);
		log.setLastName(lastName);

		session->addUndoLog(&log);
	}

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

void SchemaAlterCommandsHandler::handleAlterTableRenameTable(const AlterRenameTableCommandLog* cmd, CdbTransactionManager* trxManager) {
	CdbDatabaseSession* session = trxManager->getCdbDatabaseSession();
	UndoSpaceManager* undoManager = session->getUndoSpaceManager();
	UndoRegisterTransaction undoTrx(undoManager);

	CdbTable* table = findTableFromCommand(cmd);

	TableRenameContext context;
	context.init(cmd, this->schemaManager, nullptr);

	// undo log
	{
		UndoAlterRenameTableLog log;
		log.importFromContext(&context);

		session->addUndoLog(&log);
	}

	{
		const UnicodeString* destSchema = context.getDstSchema();
		Schema* sc = this->schemaManager->getSchema(destSchema);
		if(sc == nullptr){
			UndoCreateSchemaPart partLog;
			partLog.setSchemaName(destSchema);

			session->addUndoLogPart(&partLog);
		}
	}

	this->schemaManager->fireOnRenameTable(trxManager, table, &context);

	// upgrade
	this->schemaManager->root->upgradeSchemaObjectVersionId();
	this->schemaManager->save();

	undoTrx.commit();
}

CdbTable* SchemaAlterCommandsHandler::findTableFromCommand(const AbstractAlterCommandLog* cmdlog) {
	const TableIdentifier* tableId = cmdlog->getTableId();

	const UnicodeString* schema = tableId->getSchema();
	const UnicodeString* tableName = tableId->getTableName();

	CdbTable* table = this->schemaManager->getTable(schema, tableName);

	return table;
}

} /* namespace codablecash */
