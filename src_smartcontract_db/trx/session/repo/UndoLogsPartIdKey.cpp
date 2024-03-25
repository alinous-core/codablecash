/*
 * UndoLogsPartIdKey.cpp
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsPartIdKey.h"
#include "trx/session/repo/UndoLogsKeyFactory.h"
#include "trx/session/repo/UndoLogsIdkey.h"

#include "engine/CdbOid.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

UndoLogsPartIdKey::UndoLogsPartIdKey(const UndoLogsPartIdKey &inst) {
	this->logIdKey = dynamic_cast<UndoLogsIdkey*>(inst.logIdKey->clone());
	this->partSerial = inst.partSerial->copy();
}

UndoLogsPartIdKey::UndoLogsPartIdKey() {
	this->logIdKey = nullptr;
	this->partSerial = nullptr;
}

UndoLogsPartIdKey::~UndoLogsPartIdKey() {
	delete this->logIdKey;
	delete this->partSerial;
}

int UndoLogsPartIdKey::binarySize() const {
	BinaryUtils::checkNotNull(this->logIdKey);
	BinaryUtils::checkNotNull(this->partSerial);

	int size = sizeof(uint32_t);

	size += this->logIdKey->binarySize();
	size += this->partSerial->binarySize();

	return size;
}

void UndoLogsPartIdKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->logIdKey);
	BinaryUtils::checkNotNull(this->partSerial);

	out->putInt(UndoLogsKeyFactory::UNDO_LOGS_PART_ID_KEY);

	this->logIdKey->toBinary(out);
	this->partSerial->toBinary(out);
}

UndoLogsPartIdKey* UndoLogsPartIdKey::fromBinary(ByteBuffer *in) {
	UndoLogsPartIdKey* key = new UndoLogsPartIdKey();


	uint32_t logIdKeyType = in->getInt();
	assert(UndoLogsKeyFactory::UNDO_LOGS_ID_KEY == logIdKeyType);

	key->logIdKey = UndoLogsIdkey::fromBinary(in);
	BinaryUtils::checkNotNull(key->logIdKey);

	key->partSerial = CdbOid::fromBinary(in);
	BinaryUtils::checkNotNull(key->partSerial);

	return key;
}

int UndoLogsPartIdKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	static CdbOid::ValueCompare cmp;

	const UndoLogsPartIdKey* other = dynamic_cast<const UndoLogsPartIdKey*>(key);
	int diff = this->logIdKey->compareTo(other->logIdKey);
	if(diff != 0){
		return diff;
	}

	return cmp(this->partSerial, other->partSerial);
}

AbstractBtreeKey* UndoLogsPartIdKey::clone() const noexcept {
	return new UndoLogsPartIdKey(*this);
}

void UndoLogsPartIdKey::setLogIdKey(const UndoLogsIdkey *key) noexcept {
	delete this->logIdKey;
	this->logIdKey = dynamic_cast<UndoLogsIdkey*>(key->clone());
}

void UndoLogsPartIdKey::setPartSerial(uint64_t serial) noexcept {
	delete this->partSerial;
	this->partSerial = new CdbOid(serial);
}

const CdbDatabaseSessionId* UndoLogsPartIdKey::getSessionId() const noexcept {
	return this->logIdKey->getSessionId();
}

const CdbOid* UndoLogsPartIdKey::getUndoLogSerial() const noexcept {
	return this->logIdKey->getUndoLogSerial();
}

} /* namespace codablecash */
