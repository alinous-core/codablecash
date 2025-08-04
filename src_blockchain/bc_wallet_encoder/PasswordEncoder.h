/*
 * PasswordEncoder.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_ENCODER_PASSWORDENCODER_H_
#define BC_WALLET_ENCODER_PASSWORDENCODER_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

#include "bc_wallet_encoder/IWalletDataEncoder.h"

namespace codablecash {

class HdWalletSeed;
class Aes256CbcResult;

class PasswordEncoder : public IWalletDataEncoder {
public:
	explicit PasswordEncoder(const UnicodeString* password);
	virtual ~PasswordEncoder();

	virtual HdWalletSeed* encode(const HdWalletSeed* seed) const noexcept;
	virtual HdWalletSeed* decode(const HdWalletSeed* encodedSeed) const noexcept;

	virtual StakingEncriptedSeed* encodeStakingSource(const NodeIdentifierSource *source) const noexcept;
	virtual NodeIdentifierSource* decodeStakingSource(const StakingEncriptedSeed* encrypted) const noexcept;

	virtual IWalletDataEncoder* copy() const noexcept;

private:
	Aes256CbcResult* encode(const uint8_t* data, int size) const noexcept;
	ByteBuffer* decode(const uint8_t* b, int size) const noexcept;

private:
	UnicodeString* password;
	ByteBuffer* keybuff;
};

} /* namespace codablecash */

#endif /* BC_WALLET_ENCODER_PASSWORDENCODER_H_ */
