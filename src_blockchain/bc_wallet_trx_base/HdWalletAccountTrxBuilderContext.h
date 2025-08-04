/*
 * HdWalletAccountTrxBuilderContext.h
 *
 *  Created on: Jun 4, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_HDWALLETACCOUNTTRXBUILDERCONTEXT_H_
#define BC_WALLET_TRX_BASE_HDWALLETACCOUNTTRXBUILDERCONTEXT_H_

#include "bc_wallet_trx_base/ITransactionBuilderContext.h"

namespace codablecash {

class WalletAccount;
class IWalletDataEncoder;

class HdWalletAccountTrxBuilderContext: public ITransactionBuilderContext {
public:
	HdWalletAccountTrxBuilderContext(WalletAccount* account, const IWalletDataEncoder* encoder);
	virtual ~HdWalletAccountTrxBuilderContext();

	virtual HdWalleMuSigSignerProvidor* getMusigSignProvidor(const IWalletDataEncoder* encoder);
	virtual IUtxoCollector* getUtxoCollector();


private:
	WalletAccount* account;
	const IWalletDataEncoder* encoder;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_HDWALLETACCOUNTTRXBUILDERCONTEXT_H_ */
