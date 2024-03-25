/*
 * ZoneNodeKey.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_info/ZoneNodeKey.h"

#include "bc_network/NodeIdentifierKey.h"

#include "bc_p2p_info/ZoneNodeKeyFactory.h"

#include "base/StackRelease.h"
namespace codablecash {

ZoneNodeKey::ZoneNodeKey(const ZoneNodeKey &inst) {
	this->zone = inst.zone;
	this->key = inst.key != nullptr ? dynamic_cast<NodeIdentifierKey*>(inst.key->copyData()) : nullptr;
}

ZoneNodeKey::ZoneNodeKey() {
	this->zone = 0;
	this->key = nullptr;
}

ZoneNodeKey::~ZoneNodeKey() {
	delete this->key;
}

void ZoneNodeKey::init(uint16_t zone, const NodeIdentifierKey *key) noexcept {
	this->zone = zone;

	delete this->key;
	this->key = key != nullptr ? dynamic_cast<NodeIdentifierKey*>(key->copyData()) : nullptr;
}

int ZoneNodeKey::binarySize() const {
	int size = sizeof(uint32_t);

	size += sizeof(this->zone);

	size += sizeof(uint8_t);
	if(this->key != nullptr){
		size += this->key->binarySize();
	}

	return size;
}

void ZoneNodeKey::toBinary(ByteBuffer *out) const {
	out->putInt(ZoneNodeKeyFactory::ZONE_NODE_KEY);

	out->putShort(this->zone);

	bool isnotnull = (this->key != nullptr);
	out->put(isnotnull ? 1 : 0);

	if(isnotnull){
		this->key->toBinary(out);
	}
}

ZoneNodeKey* ZoneNodeKey::fromBinary(ByteBuffer *in) {
	uint16_t zone = in->getShort();

	NodeIdentifierKey *key = nullptr;
	uint8_t bl = in->get();
	if(bl > 0){
		uint32_t t = in->getInt();
		key = NodeIdentifierKey::fromBinary(in);
	}
	__STP(key);

	ZoneNodeKey* zkey = new ZoneNodeKey();
	zkey->init(zone, key);

	return zkey;
}

int ZoneNodeKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const ZoneNodeKey* other = dynamic_cast<const ZoneNodeKey*>(key);

	int zoneDiff = (int)this->zone - (int)other->zone;
	if(zoneDiff != 0){
		return zoneDiff;
	}

	if(other->key == nullptr || this->key == nullptr){
		return this->key == nullptr && other->key == nullptr ? 0
				: this->key != nullptr && other->key == nullptr ? 1	: -1;
	}

	return this->key->compareTo(other->key);
}

AbstractBtreeKey* ZoneNodeKey::clone() const noexcept {
	return new ZoneNodeKey(*this);
}

} /* namespace codablecash */
