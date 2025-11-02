/*
 * HdWalletAccountTrxBuilderContext.cpp
 *
 *  Created on: Jun 4, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx_base/HdWalletAccountTrxBuilderContext.h"
#include "bc_wallet_trx_base/HdWalletAccountUtxoCollector.h"

#include "bc_wallet/HdWalleMuSigSignerProvidor.h"


namespace codablecash {

HdWalletAccountTrxBuilderContext::HdWalletAccountTrxBuilderContext(WalletAccount* account, const IWalletDataEncoder* encoder) {
	this->account = account;
	this->encoder = encoder;
}

HdWalletAccountTrxBuilderContext::~HdWalletAccountTrxBuilderContext() {
	this->account = nullptr;
	this->encoder = nullptr;
}

HdWalleMuSigSignerProvidor* HdWalletAccountTrxBuilderContext::getMusigSignProvidor(const IWalletDataEncoder *encoder) {
	return new HdWalleMuSigSignerProvidor(this->account, this->encoder);
}

IUtxoCollector* HdWalletAccountTrxBuilderContext::getUtxoCollector() {
	return new HdWalletAccountUtxoCollector(this->account);
}

} /* namespace codablecash */
