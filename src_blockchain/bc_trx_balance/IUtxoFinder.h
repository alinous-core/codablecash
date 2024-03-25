/*
 * IUtxoFinder.h
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_IUTXOFINDER_H_
#define BC_TRX_BALANCE_IUTXOFINDER_H_

namespace codablecash {

class BalanceUtxo;
class UtxoId;

class IUtxoFinder {
public:
	IUtxoFinder();
	virtual ~IUtxoFinder();

	virtual BalanceUtxo* getBalanceUtxo(const UtxoId* utxoId) = 0;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_IUTXOFINDER_H_ */
