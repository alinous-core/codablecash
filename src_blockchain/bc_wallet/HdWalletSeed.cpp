/*
 * HdWalletSeed.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "numeric/BigInteger.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include <cstdint>


namespace codablecash {

const BigInteger HdWalletSeed::Q(L"ff66c4652cbb54e13e4cc75898014aef72332e147343a95031cf416ca9f77ce7", 16);
const BigInteger HdWalletSeed::G(L"e000000000000000000000000000000000000000000000000000000000000002", 16);


HdWalletSeed::HdWalletSeed() : Abstract32BytesId() {
}

HdWalletSeed::HdWalletSeed(const char *binary, int length) : Abstract32BytesId(binary, length) {
}

HdWalletSeed::~HdWalletSeed() {
}

HdWalletSeed* HdWalletSeed::newSeed() noexcept {
	int size = 0;
	BigInteger p(L"0", 16);
	do{
		BigInteger seed = BigInteger::ramdom();

		BigInteger s = seed.mod(HdWalletSeed::Q);
		p = G.modPow(s, HdWalletSeed::Q);
		size = p.binarySize();
	} while(size != 32);

	ByteBuffer* buff = p.toBinary(); __STP(buff);
	buff->position(0);

	HdWalletSeed* walletSeed = new HdWalletSeed();
	walletSeed->id = ByteBuffer::wrapWithEndian(buff->array(), size, true);

	return walletSeed;

}

IBlockObject* HdWalletSeed::copyData() const noexcept {
	return new HdWalletSeed(*this);
}

HdWalletSeed* HdWalletSeed::encodedSeed(const IWalletDataEncoder *encoder) const noexcept {
	return encoder->encode(this);
}

HdWalletSeed* HdWalletSeed::indexedSeed(int accountIndex) const noexcept {
	int cap = this->id->limit() + sizeof(uint64_t);
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);

	buff->putLong(accountIndex);

	this->id->position(0);
	buff->put(this->id);

	ByteBuffer* id = Sha256::sha256(buff, true);

	HdWalletSeed* walletSeed = new HdWalletSeed();
	walletSeed->id = id;

	return walletSeed;
}

} /* namespace codablecash */
