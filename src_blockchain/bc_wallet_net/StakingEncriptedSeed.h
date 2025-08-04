/*
 * StakingSeed.h
 *
 *  Created on: Jul 28, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_STAKINGENCRIPTEDSEED_H_
#define BC_WALLET_NET_STAKINGENCRIPTEDSEED_H_
#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;


namespace codablecash {

class StakingEncriptedSeed {
public:
	StakingEncriptedSeed(const uint8_t* binary, int length);
	virtual ~StakingEncriptedSeed();

	ByteBuffer* getByteBuffer() const noexcept {
		return this->encryptedSeed;
	}

private:
	ByteBuffer* encryptedSeed;

};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_STAKINGENCRIPTEDSEED_H_ */
