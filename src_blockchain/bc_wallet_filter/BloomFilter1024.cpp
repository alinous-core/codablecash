/*
 * BloomFilter512.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: iizuka
 */

#include "base/StackRelease.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BalanceAddress.h"
#include "bc_wallet_filter/BloomFilter1024.h"

#include "crypto/Sha256.h"

#include "bc_wallet_filter/BloomHash1024.h"
namespace codablecash {

BloomFilter1024::BloomFilter1024(const BloomFilter1024 &inst) : BloomFilter(inst) {
}

BloomFilter1024::BloomFilter1024() : BloomFilter(1024) {

}

BloomFilter1024::~BloomFilter1024() {

}

void BloomFilter1024::add(const BalanceAddress *address) {
	AddressDescriptor* addressDesc = address->toAddressDescriptor(); __STP(addressDesc);

	add(addressDesc);
}

void BloomFilter1024::add(const AddressDescriptor *addressDesc) {
	const char* str = addressDesc->toCString();
	StackArrayRelease<const char> __str(str);

	int length = ::strlen(str);

	BloomFilter::add(str, length);
}

bool BloomFilter1024::checkBytes(const BalanceAddress *address) {
	AddressDescriptor* addressDesc = address->toAddressDescriptor(); __STP(addressDesc);

	return checkBytes(addressDesc);
}

bool BloomFilter1024::checkBytes(const AddressDescriptor *addressDesc) {
	const char* str = addressDesc->toCString();
	StackArrayRelease<const char> __str(str);

	int length = ::strlen(str);

	return BloomFilter::checkBytes(str, length);
}

int BloomFilter1024::binarySize() const {
	return __binarySize();
}

void BloomFilter1024::toBinary(ByteBuffer *out) const {
	return __toBinary(out);
}

IBlockObject* BloomFilter1024::copyData() const noexcept {
	return new BloomFilter1024(*this);
}

BloomFilter1024* BloomFilter1024::createFromBinary(ByteBuffer *in) {
	BloomFilter1024* filter = new BloomFilter1024(); __STP(filter);
	filter->__fromBinary(in);

	return __STP_MV(filter);
}

/**
 * Create hash for a single address
 * @param addressDesc
 * @return
 */
BloomHash1024* BloomFilter1024::getHash(const AddressDescriptor *addressDesc) const {
	const char* str = addressDesc->toCString();
	StackArrayRelease<const char> __str(str);

	int length = ::strlen(str);

	ByteBuffer* bb = Sha256::sha256(str, length, true); __STP(bb);
	bb->position(0);

	uint64_t l = this->bitlength;

	uint64_t _v1 = ((uint64_t)bb->getLong());
	uint64_t _v2 = ((uint64_t)bb->getLong());
	uint64_t _v3 = ((uint64_t)bb->getLong());

	uint64_t v1 = _v1 % l;
	uint64_t v2 = _v2 % l;
	uint64_t v3 = _v3 % l;

	return new BloomHash1024(v1, v2, v3);
}

bool BloomFilter1024::checkBytes(const BloomHash1024 *hash) {
	uint64_t v1 = hash->getValue(0);
	uint64_t v2 = hash->getValue(1);
	uint64_t v3 = hash->getValue(2);

	return BloomFilter::checkBytes(v1, v2, v3);
}

} /* namespace codablecash */
