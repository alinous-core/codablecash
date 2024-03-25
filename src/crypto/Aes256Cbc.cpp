/*
 * Aes256Cbc.cpp
 *
 *  Created on: 2023/01/27
 *      Author: iizuka
 */

#include "crypto/Aes256Cbc.h"

#include "osenv/memory.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "aes256cbc/aes256.h"
using alinous::Mem;

namespace codablecash {

const uint8_t Aes256Cbc::DEFAULT_IV[16] = {0x86, 0xaf, 0xc4, 0x38, 0x68, 0xfe, 0xa6, 0xab, 0xd4, 0x0f, 0xbf, 0x6d, 0x5e, 0xd5, 0x09, 0x05};
const uint8_t Aes256Cbc::DEFAULT_Key256[32] = {0xf4, 0x15, 0x0d, 0x4a, 0x1a, 0xc5, 0x70, 0x8c, 0x29, 0xe4, 0x37, 0x74, 0x90, 0x45, 0xa3, 0x9a, 0x29
		, 0xe4, 0x37, 0x74, 0x1a, 0xc5, 0x70, 0x8c, 0xf4, 0x15, 0x0d, 0x4a, 0x90, 0x45, 0xa3, 0x9a};


Aes256Cbc::Aes256Cbc() {
	Mem::memcpy(this->iv, DEFAULT_IV, 16);
	Mem::memcpy(this->key256, DEFAULT_Key256, 32);
}

Aes256Cbc::~Aes256Cbc() {

}

void Aes256Cbc::setIv(const uint8_t *iv, int length) noexcept {
	Mem::memset(this->iv, 0, 16);
	Mem::memcpy(this->iv, iv, length);
}

void Aes256Cbc::setKey(const uint8_t *key256) noexcept {
	Mem::memcpy(this->key256, key256, 32);
}


Aes256CbcResult* Aes256Cbc::encryptoPlainText(const UnicodeString *str) noexcept {
	const char* cstr = str->toCString(); // utf8 String
	StackArrayRelease<const char> __st_cstr(cstr);

	return encryptoPlainText(cstr);
}

Aes256CbcResult* Aes256Cbc::encryptoPlainText(const char *cstr) noexcept {
	int str_len = Mem::strlen(cstr);

	int pkcs_length = pkcs7PaddedLength(str_len);
	uint8_t *input = pkcs7Padding(cstr, str_len, pkcs_length);

	aes256_context ctx;
	aes256_init_ex(&ctx, this->key256, this->iv);

	for(int i = 0; i != pkcs_length; i += BLOCK_SIZE){
		aes256_encrypt_cbc(&ctx, input + i);
	}

	aes256_done(&ctx);

	return new Aes256CbcResult(pkcs_length, input);
}


int Aes256Cbc::pkcs7PaddedLength(int str_len) {
	int length = (str_len / BLOCK_SIZE + 1) * BLOCK_SIZE;
	return length;
}

uint8_t* Aes256Cbc::pkcs7Padding(const char *cstr, int str_len, int pkcs_length) {
	uint8_t* buff = new uint8_t[pkcs_length];

	int i = 0;
	for(; i < str_len; ++i){
		buff[i] = cstr[i];
	}

	int remain = BLOCK_SIZE - (str_len % BLOCK_SIZE);

	for(; i != pkcs_length; ++i){
		buff[i] = remain;
	}

	return buff;
}

const char* Aes256Cbc::decrypt(const uint8_t *data, int length) {
	if(length % BLOCK_SIZE != 0){
		return nullptr;
	}

	uint8_t* input = new uint8_t[length];
	StackArrayRelease<uint8_t> __st__input(input);
	Mem::memcpy(input, data, length);

	aes256_context ctx;
	aes256_init_ex(&ctx, this->key256, this->iv);

	for(int i = 0; i != length; i += BLOCK_SIZE){
		aes256_decrypt_cbc(&ctx, input + i);
	}

	aes256_done(&ctx);

	// remove PKC
	int contentLength = length - input[length - 1];
	uint8_t* result = new uint8_t[contentLength + 1];
	result[contentLength] = 0;
	Mem::memcpy(result, input, contentLength);

	return (const char*)result;
}

} /* namespace codablecash */
