/*
 * ArrayUtxoFinder.h
 *
 *  Created on: May 24, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_ARRAYUTXOFINDER_H_
#define BC_WALLET_TRX_BASE_ARRAYUTXOFINDER_H_

#include "bc_trx_balance/IUtxoFinder.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class BalanceUtxo;
class BalanceUnit;

class ArrayUtxoFinder : public IUtxoFinder {
public:
	ArrayUtxoFinder();
	virtual ~ArrayUtxoFinder();

	void addUtxo(const BalanceUtxo* utxo) noexcept;
	virtual BalanceUtxo* getBalanceUtxo(const UtxoId* utxoId);


	BalanceUnit getTotalAmount() const noexcept;

private:
	ArrayList<BalanceUtxo>* utxoList;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_ARRAYUTXOFINDER_H_ */
