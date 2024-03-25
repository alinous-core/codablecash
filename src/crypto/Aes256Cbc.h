/*
 * Aes256Cbc.h
 *
 *  Created on: 2023/01/27
 *      Author: iizuka
 */

#ifndef CRYPTO_AES256CBC_H_
#define CRYPTO_AES256CBC_H_
#include <cstdint>

namespace alinous {
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class Aes256CbcResult{
public:
	Aes256CbcResult(int length, uint8_t* data){
		this->length = length;
		this->data = data;
	}
	~Aes256CbcResult(){
		delete [] this->data;
	}
	int length;
	uint8_t* data;
};

class Aes256Cbc {
public:
	static const uint8_t DEFAULT_IV[16];
	static const uint8_t DEFAULT_Key256[32];
	static const constexpr int BLOCK_SIZE{16};

	Aes256Cbc();
	virtual ~Aes256Cbc();

	/**
	 * iv : 16 bytes
	 */
	void setIv(const uint8_t* iv, int length) noexcept;
	/**
	 * key256 : 32bytes
	 */
	void setKey(const uint8_t* key256) noexcept;

	Aes256CbcResult* encryptoPlainText(const UnicodeString* str) noexcept;
	Aes256CbcResult* encryptoPlainText(const char* cstr) noexcept;

	const char* decrypt(const uint8_t* data, int length);

private:
	int pkcs7PaddedLength(int str_len);
	uint8_t* pkcs7Padding(const char *cstr, int str_len, int pkcs_length);

private:
	uint8_t iv[16];
	uint8_t key256[32];
};

} /* namespace codablecash */

#endif /* CRYPTO_AES256CBC_H_ */
