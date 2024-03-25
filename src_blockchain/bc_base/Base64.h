/*
 * Base64.h
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#ifndef BC_BASE_BASE64_H_
#define BC_BASE_BASE64_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class Base64 {
public:
	static const constexpr wchar_t* legalChars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	static UnicodeString* encode(const uint8_t* data, int len);
	static ByteBuffer* decode(const char* sArr, int sLen);

private:
	static const int* getIA();
};

} /* namespace codablecash */

#endif /* BC_BASE_BASE64_H_ */
