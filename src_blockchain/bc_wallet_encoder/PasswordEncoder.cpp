/*
 * PasswordEncoder.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet_encoder/PasswordEncoder.h"
#include "bc_wallet/HdWalletSeed.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "crypto/Aes256Cbc.h"

#include "bc_base/Base64.h"

#include "crypto/Sha256.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_wallet_net/StakingEncriptedSeed.h"

namespace codablecash {

PasswordEncoder::PasswordEncoder(const UnicodeString* password) {
	this->password = new UnicodeString(*password);

	{
		const char* p = this->password->toCString();
		int length = ::strlen(p);
		StackArrayRelease<const char> __st_p(p);
		this->keybuff = Sha256::sha256(p, length, true);
		this->keybuff->position(0);
	}
}

PasswordEncoder::~PasswordEncoder() {
	delete this->password;
	delete this->keybuff;
}

HdWalletSeed* PasswordEncoder::encode(const HdWalletSeed *seed) const noexcept {
	int size = seed->size();
	const char* data = seed->toArray();

	Aes256CbcResult* result = encode((const uint8_t*)data, size); __STP(result);

	return new HdWalletSeed((const char*)result->data, result->length);
}

HdWalletSeed* PasswordEncoder::decode(const HdWalletSeed *encodedSeed) const noexcept {
	ByteBuffer* buff = decode((const uint8_t*)encodedSeed->toArray(), encodedSeed->size()); __STP(buff);

	return new HdWalletSeed((const char*)buff->array(), buff->limit());
}

StakingEncriptedSeed* PasswordEncoder::encodeStakingSource(const NodeIdentifierSource *source) const noexcept {
	int size = source->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	source->toBinary(buff);

	buff->position(0);
	Aes256CbcResult* result = encode(buff->array(), buff->capacity()); __STP(result);

	return new StakingEncriptedSeed(result->data, result->length);
}

NodeIdentifierSource* PasswordEncoder::decodeStakingSource(const StakingEncriptedSeed *encrypted) const noexcept {
	ByteBuffer* encbuff = encrypted->getByteBuffer();
	encbuff->position(0);

	ByteBuffer* buff = decode(encbuff->array(), encbuff->capacity()); __STP(buff);

	buff->position(0);
	NodeIdentifierSource* source = NodeIdentifierSource::createFromBinary(buff);

	return source;
}

IWalletDataEncoder* PasswordEncoder::copy() const noexcept {
	return new PasswordEncoder(this->password);
}

Aes256CbcResult* PasswordEncoder::encode(const uint8_t *data, int size) const noexcept {
	UnicodeString* str = Base64::encode((const uint8_t*)data, size); __STP(str);

	Aes256Cbc aes;
	aes.setKey(this->keybuff->array());
	Aes256CbcResult* result = aes.encryptoPlainText(str); __STP(result);

	return __STP_MV(result);
}

ByteBuffer* PasswordEncoder::decode(const uint8_t *data, int size) const noexcept {
	Aes256Cbc aes;
	aes.setKey(this->keybuff->array());

	const char* decoded_cstr = aes.decrypt(data, size);
	StackArrayRelease<const char> __st_base64(decoded_cstr);

	int length = ::strlen(decoded_cstr);
	ByteBuffer* buff = Base64::decode(decoded_cstr, length); __STP(buff);

	return __STP_MV(buff);
}

} /* namespace codablecash */
