/*
 * BalanceAddress.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_base/BalanceAddress.h"

#include "ecda/ScPublicKey.h"
#include "ecda/Secp256k1CompressedPoint.h"

#include "crypto/Sha256.h"
#include "crypto/RipeMd160.h"

#include "base/StackRelease.h"


namespace codablecash {

const char* BalanceAddress::PREFIX = "cb";

BalanceAddress::BalanceAddress() : AbstractAddress(0) {
	this->pubkey = nullptr;
}

BalanceAddress::BalanceAddress(uint16_t zone, Secp256k1CompressedPoint* pubkey) : AbstractAddress(zone) {
	this->zone = zone;
	this->pubkey = pubkey;
}

BalanceAddress::~BalanceAddress() {
	delete this->pubkey;
}

BalanceAddress* BalanceAddress::createAddress(uint16_t zone, const ScPublicKey *publicKey) {
	Secp256k1CompressedPoint pubkey = Secp256k1CompressedPoint::compress(publicKey);
	BalanceAddress* address = new BalanceAddress(zone, dynamic_cast<Secp256k1CompressedPoint*>(pubkey.copyData()));

	return address;
}

const char* BalanceAddress::getPrefix() const noexcept {
	return BalanceAddress::PREFIX;
}

ByteBuffer* BalanceAddress::getBodyPart() const noexcept {
	int size = this->pubkey->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	this->pubkey->toBinary(buff);

	// sha256
	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	// RIPEMD-160
	ByteBuffer* ripe = RipeMd160::encode(sha);

	return ripe;
}

int BalanceAddress::binarySize() const {
	int total = sizeof(uint8_t);
	total += sizeof(this->zone);
	total += this->pubkey->binarySize();

	return total;
}

void BalanceAddress::toBinary(ByteBuffer *out) const {
	out->put(getType());

	out->putShort(this->zone);

	this->pubkey->toBinary(out);
}

void BalanceAddress::fromBinary(ByteBuffer *in) {
	this->zone = in->getShort();

	this->pubkey = Secp256k1CompressedPoint::fromBinary(in);
}

IBlockObject* BalanceAddress::copyData() const noexcept {
	return new BalanceAddress(this->zone, dynamic_cast<Secp256k1CompressedPoint*>(this->pubkey->copyData()));
}

} /* namespace codablecash */
