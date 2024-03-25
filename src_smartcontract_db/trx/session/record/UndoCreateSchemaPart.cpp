/*
 * UndoCreateSchemaPart.cpp
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#include "trx/session/record/UndoCreateSchemaPart.h"

#include "base/UnicodeString.h"

#include "bc_base/BinaryUtils.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table_store/CdbStorageManager.h"

#include "base/StackRelease.h"

#include "base_io/File.h"
namespace codablecash {

UndoCreateSchemaPart::UndoCreateSchemaPart(const UndoCreateSchemaPart &inst) : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_CREATE_SCHEMA) {
	this->schemaName = inst.schemaName != nullptr ? new UnicodeString(inst.schemaName) : nullptr;
}

UndoCreateSchemaPart::UndoCreateSchemaPart() : AbstractUndoLogPart(AbstractUndoLogPart::PART_UNDO_CREATE_SCHEMA){
	this->schemaName = nullptr;
}

UndoCreateSchemaPart::~UndoCreateSchemaPart() {
	delete this->schemaName;
}

int UndoCreateSchemaPart::binarySize() const {
	BinaryUtils::checkNotNull(this->schemaName);

	int total = sizeof(uint8_t);
	total += BinaryUtils::stringSize(this->schemaName);

	return total;
}

void UndoCreateSchemaPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->schemaName);

	out->put(type);
	BinaryUtils::putString(out, this->schemaName);
}

void UndoCreateSchemaPart::fromBinary(ByteBuffer *in) {
	this->schemaName = BinaryUtils::getString(in);
	BinaryUtils::checkNotNull(this->schemaName);
}

IBlockObject* UndoCreateSchemaPart::copyData() const noexcept {
	return new UndoCreateSchemaPart(*this);
}

void UndoCreateSchemaPart::execute(CdbTransactionManager *trxManager, TableStore *store) const {
	SchemaManager* schemaManager = trxManager->getSchemaManager();

	schemaManager->removeSchema(this->schemaName);

	schemaManager->save();

	CdbStorageManager* storeManager = trxManager->getStorageManager();

	const File* baseDir = schemaManager->getDatabaseBaseDir();
	File* schemaDir = baseDir->get(this->schemaName); __STP(schemaDir);
	bool bl = schemaDir->deleteDir();

	assert(bl);
}

void UndoCreateSchemaPart::setSchemaName(const UnicodeString *name) noexcept {
	delete this->schemaName;
	this->schemaName = new UnicodeString(name);
}

} /* namespace codablecash */
