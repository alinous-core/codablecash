/*
 * HdWalleMuSigSignerProvidor.h
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#ifndef BC_WALLET_HDWALLEMUSIGSIGNERPROVIDOR_H_
#define BC_WALLET_HDWALLEMUSIGSIGNERPROVIDOR_H_

#include "bc_trx_balance/IMuSigSignerProvidor.h"

namespace codablecash {

class IWalletDataEncoder;
class WalletAccount;

class HdWalleMuSigSignerProvidor: public IMuSigSignerProvidor {
public:
	HdWalleMuSigSignerProvidor(WalletAccount* account, const IWalletDataEncoder* encoder);
	virtual ~HdWalleMuSigSignerProvidor();

	virtual IMuSigSigner* getSigner(const AddressDescriptor* desc);

private:
	WalletAccount* account;
	IWalletDataEncoder* encoder;
};

} /* namespace codablecash */

#endif /* BC_WALLET_HDWALLEMUSIGSIGNERPROVIDOR_H_ */
