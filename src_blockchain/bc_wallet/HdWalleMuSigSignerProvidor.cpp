/*
 * HdWalleMuSigSignerProvidor.cpp
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#include "bc_wallet/HdWalleMuSigSignerProvidor.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

namespace codablecash {

HdWalleMuSigSignerProvidor::HdWalleMuSigSignerProvidor(WalletAccount* account, const IWalletDataEncoder* encoder) {
	this->account = account;
	this->encoder = encoder->copy();
}

HdWalleMuSigSignerProvidor::~HdWalleMuSigSignerProvidor() {
	delete this->encoder;
}

IMuSigSigner* HdWalleMuSigSignerProvidor::getSigner(const AddressDescriptor *desc) {
	return this->account->getSigner(desc, this->encoder);
}

} /* namespace codablecash */
