/*
 * ITransactionBuilderContext.h
 *
 *  Created on: May 11, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_ITRANSACTIONBUILDERCONTEXT_H_
#define BC_WALLET_TRX_ITRANSACTIONBUILDERCONTEXT_H_

namespace codablecash {

class HdWalleMuSigSignerProvidor;
class IUtxoCollector;
class IWalletDataEncoder;

class ITransactionBuilderContext {
public:
	ITransactionBuilderContext();
	virtual ~ITransactionBuilderContext();

	virtual HdWalleMuSigSignerProvidor* getMusigSignProvidor(const IWalletDataEncoder* encoder) = 0;
	virtual IUtxoCollector* getUtxoCollector() = 0;

};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_ITRANSACTIONBUILDERCONTEXT_H_ */
