/*
 * PasswordEncoder.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_ENCODER_PASSWORDENCODER_H_
#define BC_WALLET_ENCODER_PASSWORDENCODER_H_

namespace alinous {
class UnicodeString;
class ByteBuffer;
}
using namespace alinous;

#include "bc_wallet_encoder/IWalletDataEncoder.h"

namespace codablecash {

class HdWalletSeed;

class PasswordEncoder : public IWalletDataEncoder {
public:
	explicit PasswordEncoder(const UnicodeString* password);
	virtual ~PasswordEncoder();

	virtual HdWalletSeed* encode(const HdWalletSeed* seed) const noexcept;
	virtual HdWalletSeed* decode(const HdWalletSeed* encodedSeed) const noexcept;

	virtual IWalletDataEncoder* copy() const noexcept;

private:
	UnicodeString* password;
	ByteBuffer* keybuff;
};

} /* namespace codablecash */

#endif /* BC_WALLET_ENCODER_PASSWORDENCODER_H_ */
