/*
 * RipeMd160.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef CRYPTO_RIPEMD160_H_
#define CRYPTO_RIPEMD160_H_

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class RipeMd160 {
public:
	static ByteBuffer* encode(ByteBuffer* inbuff);
};

} /* namespace codablecash */

#endif /* CRYPTO_RIPEMD160_H_ */
