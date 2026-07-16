/*
 * WalletMemoryPoolScanner.h
 *
 *  Created on: Jun 9, 2026
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_WALLETMEMORYPOOLSCANNER_H_
#define BC_WALLET_NET_DATA_WALLETMEMORYPOOLSCANNER_H_

namespace alinous {
class BtreeScanner;
}
using namespace alinous;


namespace codablecash {

class WalletMemoryPool;
class TransactionId;
class WalletTransactionIdListData;
class AbstractBlockchainTransaction;

class WalletMemoryPoolScanner {
public:
	explicit WalletMemoryPoolScanner(const WalletMemoryPool* pool);
	virtual ~WalletMemoryPoolScanner();

	void begin();

	bool hasNext();
	AbstractBlockchainTransaction* next();

private:
	const WalletMemoryPool* pool;
	BtreeScanner* scanner;
	WalletTransactionIdListData* data;
	const TransactionId* trxId;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_WALLETMEMORYPOOLSCANNER_H_ */
