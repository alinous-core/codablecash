/*
 * UndoDropTableLog.cpp
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#include "trx/session/ddl/UndoDropTableLog.h"
#include "trx/session/ddl/UndoIndexPart.h"

#include "trx/session/record/UndoDeleteRecordPart.h"

#include "trx/session/base/CdbDatabaseSession.h"

#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogPartsScanner.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "snapshot/TableStartSnapshotCommand.h"
#include "snapshot/SnapshotIndexCommand.h"
#include "snapshot/TableEndSnapshotCommand.h"
#include "snapshot/SchemaStartSnapshotCommand.h"
#include "snapshot/SchemaEndSnapshotCommand.h"
#include "snapshot/SnapshotRecordCommand.h"

#include "engine/CdbOid.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/TableObjectFactory.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "schema_table/schema/SchemaManager.h"

#include "bc_base/BinaryUtils.h"

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

UndoDropTableLog::UndoDropTableLog(const UndoDropTableLog &inst) : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_DROP_TABLE) {
	this->oid = inst.oid != nullptr ? inst.oid->copy() : nullptr;
	this->name = inst.name != nullptr ? new UnicodeString(inst.name) : nullptr;
	this->schemaName = inst.schemaName != nullptr ? new UnicodeString(inst.schemaName) : nullptr;

	{
		this->columns = new ArrayList<CdbTableColumn>();
		int maxLoop = inst.columns->size();
		for(int i = 0; i != maxLoop; ++i){
			CdbTableColumn* col = inst.columns->get(i);
			this->columns->addElement(new CdbTableColumn(*col));
		}
	}

	{
		this->indexes = new ArrayList<UndoIndexPart>();
		int maxLoop = inst.indexes->size();
		for(int i = 0; i != maxLoop; ++i){
			UndoIndexPart* index = inst.indexes->get(i);
			this->indexes->addElement(new UndoIndexPart(*index));
		}
	}

	this->schemaStarted = inst.schemaStarted;
	this->tableStarted = inst.tableStarted;
	this->addedlog = inst.addedlog;

	this->dbsession = inst.dbsession;
}

UndoDropTableLog::UndoDropTableLog() : AbstractUndoLog(AbstractUndoLog::UNDO_TYPE_DROP_TABLE) {
	this->oid = nullptr;
	this->name = nullptr;
	this->schemaName = nullptr;

	this->columns = new ArrayList<CdbTableColumn>();
	this->indexes = new ArrayList<UndoIndexPart>();

	this->schemaStarted = false;
	this->tableStarted = false;
	this->addedlog = false;

	this->dbsession = nullptr;
}

UndoDropTableLog::~UndoDropTableLog() {
	delete this->oid;
	delete this->name;
	delete this->schemaName;

	this->columns->deleteElements();
	delete this->columns;

	this->indexes->deleteElements();
	delete this->indexes;

	this->dbsession = nullptr;
}

int UndoDropTableLog::binarySize() const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);
	BinaryUtils::checkNotNull(this->schemaName);

	int total = sizeof(uint8_t);

	total += this->oid->binarySize();
	total += BinaryUtils::stringSize(this->name);
	total += BinaryUtils::stringSize(this->schemaName);

	{
		int maxLoop = this->columns->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			CdbTableColumn* col = this->columns->get(i);

			total += col->binarySize();
		}
	}

	{
		int maxLoop = this->indexes->size();
		total += sizeof(uint16_t);

		for(int i = 0; i != maxLoop; ++i){
			UndoIndexPart* part = this->indexes->get(i);

			total += part->binarySize();
		}
	}

	return total;
}

void UndoDropTableLog::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);
	BinaryUtils::checkNotNull(this->schemaName);

	out->put(this->type);

	this->oid->toBinary(out);
	BinaryUtils::putString(out, this->name);
	BinaryUtils::putString(out, this->schemaName);

	{
		int maxLoop = this->columns->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			CdbTableColumn* col = this->columns->get(i);

			col->toBinary(out);
		}
	}

	{
		int maxLoop = this->indexes->size();
		out->putShort(maxLoop);

		for(int i = 0; i != maxLoop; ++i){
			UndoIndexPart* part = this->indexes->get(i);

			part->toBinary(out);
		}
	}
}

void UndoDropTableLog::fromBinary(ByteBuffer *in) {
	this->oid = CdbOid::fromBinary(in);
	BinaryUtils::checkNotNull(this->oid);

	this->name = BinaryUtils::getString(in);
	BinaryUtils::checkNotNull(this->name);

	this->schemaName = BinaryUtils::getString(in);
	BinaryUtils::checkNotNull(this->schemaName);

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			CdbBinaryObject* obj = TableObjectFactory::createFromBinary(in, CdbTableColumn::CDB_OBJ_TYPE);
			CdbTableColumn* col = dynamic_cast<CdbTableColumn*>(obj);
			BinaryUtils::checkNotNull(col);

			col->fromBinary(in);

			this->columns->addElement(col);
		}
	}

	{
		int maxLoop = in->getShort();
		for(int i = 0; i != maxLoop; ++i){
			UndoIndexPart* part = UndoIndexPart::fromBinary(in);
			BinaryUtils::checkNotNull(part);

			this->indexes->addElement(part);
		}
	}
}

IBlockObject* UndoDropTableLog::copyData() const noexcept {
	return new UndoDropTableLog(*this);
}

void UndoDropTableLog::putCommand(const AbstractSnapshotCommand *command) {
	if(!this->schemaStarted){
		const SchemaStartSnapshotCommand* cmd = dynamic_cast<const SchemaStartSnapshotCommand*>(command);
		if(cmd != nullptr){
			const UnicodeString* name = cmd->getName();
			if(this->schemaName->equals(name)){
				this->schemaStarted = true;
			}
		}
	}
	else{
		// check schema end
		const SchemaEndSnapshotCommand* cmd = dynamic_cast<const SchemaEndSnapshotCommand*>(command);
		if(cmd != nullptr){
			this->schemaStarted = false;
			registerUndoLog();
			return;
		}

		afterSchemaDetected(command);
	}
}

void UndoDropTableLog::afterSchemaDetected(const AbstractSnapshotCommand *command) {
	if(!this->tableStarted){
		const TableStartSnapshotCommand* cmd = dynamic_cast<const TableStartSnapshotCommand*>(command);
		if(cmd != nullptr){
			const CdbOid* oid = cmd->getOid();
			if(this->oid->equals(oid)){
				this->tableStarted = true;
				importTalbeColumns(cmd);
			}
			return;
		}
	}
	else{
		afterTableDetected(command);
	}
}

void UndoDropTableLog::importTalbeColumns(const TableStartSnapshotCommand *cmd) {
	const ArrayList<CdbTableColumn>* list = cmd->getColumns();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableColumn* col = list->get(i);

		this->columns->addElement(new CdbTableColumn(*col));
	}
}

void UndoDropTableLog::afterTableDetected(const AbstractSnapshotCommand *command) {
	{
		const TableEndSnapshotCommand* cmd = dynamic_cast<const TableEndSnapshotCommand*>(command);
		if(cmd != nullptr){
			registerUndoLog();
			this->tableStarted = false;
			return;
		}
	}

	{
		const SnapshotIndexCommand* cmd = dynamic_cast<const SnapshotIndexCommand*>(command);
		if(cmd != nullptr){
			importIndex(cmd);
			return;
		}

		registerUndoLog();

		const SnapshotRecordCommand* recordCommand = dynamic_cast<const SnapshotRecordCommand*>(command);
		if(recordCommand != nullptr){
			importRecordCommand(recordCommand);
		}
	}
}

void UndoDropTableLog::setDbSession(CdbDatabaseSession *session) noexcept {
	this->dbsession = session;
}

void UndoDropTableLog::importIndex(const SnapshotIndexCommand *cmd) {
	UndoIndexPart* indexPart = new UndoIndexPart(); __STP(indexPart);
	indexPart->setOid(cmd->getOid());
	indexPart->setName(cmd->getName());
	indexPart->setPrimary(cmd->isPrimary());
	indexPart->setUnique(cmd->isUnique());

	const ArrayList<CdbOid>* list = cmd->getColumns();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = list->get(i);

		indexPart->addColumn(col);
	}

	this->indexes->addElement(__STP_MV(indexPart));
}

void UndoDropTableLog::importRecordCommand(const SnapshotRecordCommand *recordCommand) {
	const CdbRecord* record = recordCommand->getRecord();

	UndoDeleteRecordPart part;
	part.setRecord(record);

	this->dbsession->addUndoLogPart(&part);
}

void UndoDropTableLog::setOid(const CdbOid* oid) noexcept {
	delete this->oid;
	this->oid = oid->copy();
}

void UndoDropTableLog::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(name);
}

void UndoDropTableLog::setSchemaName(const UnicodeString *schemaName) noexcept {
	delete this->schemaName;
	this->schemaName = new UnicodeString(schemaName);
}

void UndoDropTableLog::registerUndoLog() noexcept {
	if(!this->addedlog){
		this->dbsession->addUndoLog(this);
		this->addedlog = true;
	}
}

void UndoDropTableLog::execute(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key) {
	SchemaManager* schemaManager = trxManager->getSchemaManager();
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	// recover table
	{
		CdbTable* table = new CdbTable(this->oid->getOidValue()); __STP(table);
		table->setSchemaName(new UnicodeString(*this->schemaName));
		table->setName(new UnicodeString(*this->name));

		int maxLoop = this->columns->size();
		for(int i = 0; i != maxLoop; ++i){
			CdbTableColumn* col = this->columns->get(i);

			table->addColumn(new CdbTableColumn(*col));
		}

		schemaManager->recoverTable(table);
	}

	// recover indexes
	{
		int maxLoop = this->indexes->size();
		for(int i = 0; i != maxLoop; ++i){
			UndoIndexPart* indexPart = this->indexes->get(i);

			recoverIndexes(trxManager, indexPart, schemaManager);
		}
	}

	// records
	recoverRecords(trxManager, partsRepo, key);
}

void UndoDropTableLog::recoverIndexes(CdbTransactionManager* trxManager, UndoIndexPart *indexPart, SchemaManager *schemaManager) {
	CdbTable* table = schemaManager->getTable(this->oid);

	CdbTableIndex* newIndex = new CdbTableIndex(indexPart->getOid()->getOidValue());

	newIndex->setName(new UnicodeString(indexPart->getName()));
	newIndex->setPrimaryKey(indexPart->isPrimary());
	newIndex->setUnique(indexPart->isUnique());

	const ArrayList<CdbOid>* list = indexPart->getColumns();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* colOid = list->get(i);

		newIndex->addColumn(colOid);
	}

	schemaManager->recoverIndex(trxManager, newIndex, this->schemaName, this->name);
}

void UndoDropTableLog::recoverRecords(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key) {
	CdbStorageManager* storageManager = trxManager->getStorageManager();

	TableStore* store = storageManager->getTableStore(this->oid);

	const CdbDatabaseSessionId* sessionId = key->getSessionId();
	uint64_t undoLogSerial = key->getUndoLogSerial()->getOidValue();

	UndoLogPartsScanner* scanner = partsRepo->getScanner(sessionId, undoLogSerial); __STP(scanner);
	while(scanner->hasNext()){
		const AbstractUndoLogPart* part = scanner->next();

		part->execute(trxManager, store);
	}
}

} /* namespace codablecash */
