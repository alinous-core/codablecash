/*
 * BloomFilter512.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: iizuka
 */

#include "bc_wallet_filter/BloomFilter512.h"


#include "base/StackRelease.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BalanceAddress.h"


namespace codablecash {

BloomFilter512::BloomFilter512(const BloomFilter512 &inst) : BloomFilter(inst) {
}

BloomFilter512::BloomFilter512() : BloomFilter(512) {

}

BloomFilter512::~BloomFilter512() {

}

void BloomFilter512::add(const BalanceAddress *address) {
	AddressDescriptor* addressDesc = address->toAddressDescriptor(); __STP(addressDesc);

	add(addressDesc);
}

void BloomFilter512::add(const AddressDescriptor *addressDesc) {
	const char* str = addressDesc->toCString();
	StackArrayRelease<const char> __str(str);

	int length = ::strlen(str);

	BloomFilter::add(str, length);
}

bool BloomFilter512::checkBytes(const BalanceAddress *address) {
	AddressDescriptor* addressDesc = address->toAddressDescriptor(); __STP(addressDesc);

	return checkBytes(addressDesc);
}

bool BloomFilter512::checkBytes(const AddressDescriptor *addressDesc) {
	const char* str = addressDesc->toCString();
	StackArrayRelease<const char> __str(str);

	int length = ::strlen(str);

	return BloomFilter::checkBytes(str, length);
}

int BloomFilter512::binarySize() const {
	return __binarySize();
}

void BloomFilter512::toBinary(ByteBuffer *out) const {
	return __toBinary(out);
}

IBlockObject* BloomFilter512::copyData() const noexcept {
	return new BloomFilter512(*this);
}

BloomFilter512* BloomFilter512::createFromBinary(ByteBuffer *in) {
	BloomFilter512* filter = new BloomFilter512(); __STP(filter);
	filter->__fromBinary(in);

	return __STP_MV(filter);
}

} /* namespace codablecash */
