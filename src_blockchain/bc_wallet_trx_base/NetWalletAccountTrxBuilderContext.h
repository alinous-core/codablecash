/*
 * NetWalletAccountTrxBuilderContext.h
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_NETWALLETACCOUNTTRXBUILDERCONTEXT_H_
#define BC_WALLET_TRX_BASE_NETWALLETACCOUNTTRXBUILDERCONTEXT_H_

#include "bc_wallet_trx_base/ITransactionBuilderContext.h"

namespace codablecash {

class WalletAccount;
class ManagementAccountsCollection;


class NetWalletAccountTrxBuilderContext : public ITransactionBuilderContext {
public:
	NetWalletAccountTrxBuilderContext(WalletAccount* account, const IWalletDataEncoder* encoder, ManagementAccountsCollection* managementAccounts);
	virtual ~NetWalletAccountTrxBuilderContext();

	virtual HdWalleMuSigSignerProvidor* getMusigSignProvidor(const IWalletDataEncoder* encoder);
	virtual IUtxoCollector* getUtxoCollector();

private:
	WalletAccount* account;
	const IWalletDataEncoder* encoder;
	ManagementAccountsCollection* managementAccounts;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_NETWALLETACCOUNTTRXBUILDERCONTEXT_H_ */
