/*
 * SnapshotRecordCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/SnapshotRecordCommand.h"
#include "snapshot/ISnapshotRecoverer.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/table_store/CdbStorageManager.h"

#include "engine/CodableDatabase.h"

#include "snapshot/TableStartSnapshotCommand.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/table_store/TableStore.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "base/StackRelease.h"
namespace codablecash {

SnapshotRecordCommand::SnapshotRecordCommand() : AbstractSnapshotCommand(AbstractSnapshotCommand::RECORD) {
	this->record = nullptr;
}

SnapshotRecordCommand::~SnapshotRecordCommand() {
	delete this->record;
}

void SnapshotRecordCommand::setRecord(const CdbRecord *rec) noexcept {
	this->record = dynamic_cast<CdbRecord*>(rec->copy());
}

int SnapshotRecordCommand::binarySize() const {
	int total = sizeof(uint8_t);

	total += this->record->binarySize();

	return total;
}

void SnapshotRecordCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);

	this->record->toBinary(out);
}

void SnapshotRecordCommand::fromBinary(ByteBuffer *in) {
	this->record = CdbRecord::createFromBinary(in);
}

void SnapshotRecordCommand::recover(ISnapshotRecoverer *recoverer, CodableDatabase *db) {
	CdbStorageManager* storeManager = db->getStorageManager();

	TableStartSnapshotCommand* tableCmd = recoverer->getTableStartSnapshotCommand();
	CdbTable* table = tableCmd->getTable();
	const CdbOid* tableoid = table->getOid();

	TableStore* store = storeManager->getTableStore(tableoid);
	store->insert(this->record);
}

void SnapshotRecordCommand::__testCheckEquals(const AbstractSnapshotCommand *other) const {
	DatabaseEqualsChecker::checkIntEquals(this->type, other->getType());

	const SnapshotRecordCommand* cmd = dynamic_cast<const SnapshotRecordCommand*>(other);

	CdbRecordKey* key = dynamic_cast<CdbRecordKey*>(this->record->toKey()); __STP(key);
	CdbRecordKey* keyB = dynamic_cast<CdbRecordKey*>(cmd->record->toKey()); __STP(keyB);

	DatabaseEqualsChecker::checkRecordEquals(key, keyB);

}

} /* namespace codablecash */
