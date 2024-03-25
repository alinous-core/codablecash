/*
 * Sha3256.h
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#ifndef ASTROBWT_SHA3256_H_
#define ASTROBWT_SHA3256_H_

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class Sha3_256 {
public:
	//static ByteBuffer* sha3_256(ByteBuffer* buff, bool bigEndian) noexcept;
	static ByteBuffer* sha3_256(const char* binary, int length, bool bigEndian) noexcept;

};

} /* namespace codablecash */

#endif /* ASTROBWT_SHA3256_H_ */
