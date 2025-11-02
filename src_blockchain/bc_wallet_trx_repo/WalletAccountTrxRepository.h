/*
 * TrxRepository.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_WALLETACCOUNTTRXREPOSITORY_H_
#define BC_WALLET_TRX_REPO_WALLETACCOUNTTRXREPOSITORY_H_

#include "base/ArrayList.h"

#include "bc_trx_balance/IUtxoFinder.h"

namespace alinous {
class UnicodeString;
class DiskCacheManager;
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class WalletAccount;
class WalletAccountUtxoRepository;
class BalanceUtxo;
class AbstractBlockchainTransaction;
class BalanceUnit;
class AddressDescriptor;
class UtxoId;
class TransactionId;
class AbstractUtxo;


class WalletAccountTrxRepository : public IUtxoFinder {
public:
	static const UnicodeString FILE_NAME;

	explicit WalletAccountTrxRepository(WalletAccount* account);
	virtual ~WalletAccountTrxRepository();

	void init();
	void open();
	void close();

	void importUtxo(const AbstractUtxo* utxo);
	void removeUtxo(const UtxoId* utxoId);

	void importTransaction(const AbstractBlockchainTransaction *trx);
	AbstractBlockchainTransaction* findTransaction(const TransactionId* trxId);
	void removeTransaction(const TransactionId* trxId);

	BalanceUnit getTotalAmount() const noexcept;

	virtual BalanceUtxo* getBalanceUtxo(const UtxoId* utxoId);
	ArrayList<BalanceUtxo>* getBalanceUtxos(const AddressDescriptor* desc);

	AbstractUtxo* getUtxo(const UtxoId* utxoId);

	BtreeScanner* getScanner() const;

	WalletAccountUtxoRepository* getUtxoRepo() const noexcept {
		return this->utxoRepo;
	}

private:
	void initSelf();
	void openSelf();
	void closeSelf();
private:
	WalletAccount* account;

	DiskCacheManager* cacheManager;
	Btree* btree;

	WalletAccountUtxoRepository* utxoRepo;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_WALLETACCOUNTTRXREPOSITORY_H_ */
