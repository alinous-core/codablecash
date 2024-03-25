/*
 * BinaryUtils.h
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#ifndef BC_BASE_BINARYUTILS_H_
#define BC_BASE_BINARYUTILS_H_

#include <cstdint>

namespace alinous {
class IBlockObject;
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

#define __ASSERT_POS(buff) \
	assert(buff->limit() == buff->position());


namespace codablecash {

class BinaryUtils {
public:
	static void checkNotNull(void* obj);
	static void checkUShortRange(uint16_t value, uint16_t min, uint16_t max);

	static void checkUint8Value(uint8_t value, uint8_t correctValue);

	static int stringSize(const UnicodeString* str) noexcept;
	static void putString(ByteBuffer* out, const UnicodeString* str) noexcept;
	static UnicodeString* getString(ByteBuffer* in) noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_BINARYUTILS_H_ */
