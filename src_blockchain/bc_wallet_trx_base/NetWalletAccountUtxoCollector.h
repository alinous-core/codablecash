/*
 * NetWalletAccountUtxoCollector.h
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOCOLLECTOR_H_
#define BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOCOLLECTOR_H_

#include "bc_wallet_trx_base/IUtxoCollector.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class ManagementAccount;
class NetWalletAccountUtxoArray;

class NetWalletAccountUtxoCollector : public IUtxoCollector {
public:
	explicit NetWalletAccountUtxoCollector(const ManagementAccount* managementAccount);
	virtual ~NetWalletAccountUtxoCollector();

	void init();

	virtual bool hasNext();
	virtual BalanceUtxo* next();

private:
	const ManagementAccount* managementAccount;
	ArrayList<NetWalletAccountUtxoArray>* list;
	BalanceUtxo* nextUtxo;

	int utxoArrayIndex;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_NETWALLETACCOUNTUTXOCOLLECTOR_H_ */
