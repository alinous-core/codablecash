/*
 * HdWalletAccountUtxoCollector.h
 *
 *  Created on: Jun 4, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_BASE_HDWALLETACCOUNTUTXOCOLLECTOR_H_
#define BC_WALLET_TRX_BASE_HDWALLETACCOUNTUTXOCOLLECTOR_H_

#include "bc_wallet_trx_base/IUtxoCollector.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class ReceivingAddressStore;
class ChangeAddressStore;
class WalletAccount;
class WalletAccountTrxRepository;
class BalanceUtxo;

class HdWalletAccountUtxoCollector : public IUtxoCollector {
public:
	explicit HdWalletAccountUtxoCollector(WalletAccount* account);
	virtual ~HdWalletAccountUtxoCollector();

	virtual bool hasNext();
	virtual BalanceUtxo* next();

private:
	void importReceivingUtxos();
	void importChangeUtxos();

private:
	WalletAccountTrxRepository *trxRepo;
	ReceivingAddressStore* receivingAddresses;
	ChangeAddressStore* changeAddresses;

	bool changeaddr;
	ArrayList<BalanceUtxo>* butxoList;
	int cursor;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_BASE_HDWALLETACCOUNTUTXOCOLLECTOR_H_ */
