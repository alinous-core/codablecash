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

	UnicodeString* str = Base64::encode((const uint8_t*)data, size); __STP(str);

	Aes256Cbc aes;
	aes.setKey(this->keybuff->array());
	Aes256CbcResult* result = aes.encryptoPlainText(str); __STP(result);


	return new HdWalletSeed((const char*)result->data, result->length);
}

HdWalletSeed* PasswordEncoder::decode(const HdWalletSeed *encodedSeed) const noexcept {
	Aes256Cbc aes;
	aes.setKey(this->keybuff->array());

	const char* decoded_cstr = aes.decrypt((const uint8_t*)encodedSeed->toArray(), encodedSeed->size());
	StackArrayRelease<const char> __st_base64(decoded_cstr);

	int length = ::strlen(decoded_cstr);
	ByteBuffer* buff = Base64::decode(decoded_cstr, length); __STP(buff);

	return new HdWalletSeed((const char*)buff->array(), buff->limit());
}

IWalletDataEncoder* PasswordEncoder::copy() const noexcept {
	return new PasswordEncoder(this->password);
}

} /* namespace codablecash */
