/*
 * NetWalletAccountTrxBuilderContext.cpp
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */

#include "bc_wallet_trx_base/NetWalletAccountTrxBuilderContext.h"
#include "bc_wallet_trx_base/NetWalletAccountUtxoCollector.h"

#include "bc_wallet/HdWalleMuSigSignerProvidor.h"

#include "bc_wallet_net_data_management/ManagementAccountsCollection.h"


namespace codablecash {

NetWalletAccountTrxBuilderContext::NetWalletAccountTrxBuilderContext(WalletAccount* account, const IWalletDataEncoder* encoder, ManagementAccountsCollection* managementAccounts) {
	this->account = account;
	this->encoder = encoder;
	this->managementAccounts = managementAccounts;
}

NetWalletAccountTrxBuilderContext::~NetWalletAccountTrxBuilderContext() {
	this->account = nullptr;
	this->encoder = nullptr;
}

HdWalleMuSigSignerProvidor* NetWalletAccountTrxBuilderContext::getMusigSignProvidor(const IWalletDataEncoder *encoder) {
	return new HdWalleMuSigSignerProvidor(this->account, this->encoder);
}

IUtxoCollector* NetWalletAccountTrxBuilderContext::getUtxoCollector() {
	ManagementAccount* managementAccount = this->managementAccounts->getMempoolManagementAccount();

	NetWalletAccountUtxoCollector* collector = new NetWalletAccountUtxoCollector(managementAccount);
	collector->init();

	return collector;
}

} /* namespace codablecash */
