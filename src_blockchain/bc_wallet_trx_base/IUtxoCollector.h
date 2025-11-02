/*
 * IUtxoCollector.h
 *
 *  Created on: May 11, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_IUTXOCOLLECTOR_H_
#define BC_WALLET_TRX_IUTXOCOLLECTOR_H_

namespace codablecash {

class BalanceUtxo;

class IUtxoCollector {
public:
	IUtxoCollector();
	virtual ~IUtxoCollector();

	virtual bool hasNext() = 0;
	virtual BalanceUtxo* next() = 0;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_IUTXOCOLLECTOR_H_ */
