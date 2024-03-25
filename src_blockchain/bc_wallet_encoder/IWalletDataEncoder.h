/*
 * IWalletDataEncoder.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_ENCODER_IWALLETDATAENCODER_H_
#define BC_WALLET_ENCODER_IWALLETDATAENCODER_H_

namespace codablecash {

class HdWalletSeed;

class IWalletDataEncoder {
public:
	IWalletDataEncoder();
	virtual ~IWalletDataEncoder();

	virtual HdWalletSeed* encode(const HdWalletSeed* seed) const noexcept = 0;
	virtual HdWalletSeed* decode(const HdWalletSeed* encodedSeed) const noexcept = 0;

	virtual IWalletDataEncoder* copy() const noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_WALLET_ENCODER_IWALLETDATAENCODER_H_ */
