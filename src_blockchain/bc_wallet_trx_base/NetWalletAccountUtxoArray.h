/*
 * NetWalletAccountUtxoArray.h
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOARRAY_H_
#define BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOARRAY_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class BalanceUtxo;

class NetWalletAccountUtxoArray {
public:
	NetWalletAccountUtxoArray();
	virtual ~NetWalletAccountUtxoArray();

	void addBalanceUtxo(const BalanceUtxo* utxo) noexcept;

	bool hasNext() noexcept;
	const BalanceUtxo* next() noexcept;

private:
	ArrayList<BalanceUtxo>* list;
	int pos;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOARRAY_H_ */
