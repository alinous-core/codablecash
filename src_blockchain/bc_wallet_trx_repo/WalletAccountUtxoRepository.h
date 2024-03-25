/*
 * WalletAccountUtxo.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_WALLETACCOUNTUTXOREPOSITORY_H_
#define BC_WALLET_TRX_REPO_WALLETACCOUNTUTXOREPOSITORY_H_
#include "base/ArrayList.h"

namespace alinous {
class File;
class UnicodeString;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class BalanceUtxo;
class BalanceUnit;
class AddressDescriptor;
class UtxoId;

class WalletAccountUtxoRepository {
public:
	static const UnicodeString FILE_NAME;
	static const UnicodeString UTXO_ID_INDEX_FILE_NAME;

	explicit WalletAccountUtxoRepository(const File* accountBaseDir);
	virtual ~WalletAccountUtxoRepository();

	void init();

	void open();
	void close() noexcept;

	void importUtxo(const BalanceUtxo* utxo);
	BalanceUtxo* getBalanceUtxo(const UtxoId* utxoId);
	void removeUtxo(const UtxoId* utxoId);

	BalanceUnit getTotalAmount() const noexcept;
	ArrayList<BalanceUtxo>* getBalanceUtxos(const AddressDescriptor* desc);


private:
	const File* accountBaseDir;
	DiskCacheManager* cacheManager;
	Btree* btree;
	Btree* utxoIdBtree;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_WALLETACCOUNTUTXOREPOSITORY_H_ */
