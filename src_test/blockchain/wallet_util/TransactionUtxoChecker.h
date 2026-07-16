/*
 * TransactionUtxoChecker.h
 *
 *  Created on: Jun 5, 2026
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_WALLET_UTIL_TRANSACTIONUTXOCHECKER_H_
#define BLOCKCHAIN_WALLET_UTIL_TRANSACTIONUTXOCHECKER_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;
class AbstractUtxoReference;
class UtxoId;


class TransactionUtxoChecker {
public:
	TransactionUtxoChecker();
	virtual ~TransactionUtxoChecker();

	bool checkTransaction(const AbstractBlockchainTransaction* trx);

	void addTrx(const AbstractBlockchainTransaction* trx);

private:
	bool checkDoubleUseUtxo(const AbstractUtxoReference* utxoRef);
	bool hasUtxoRef(const AbstractBlockchainTransaction* trx, const UtxoId* id);

private:
	ArrayList<AbstractBlockchainTransaction>* list;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_WALLET_UTIL_TRANSACTIONUTXOCHECKER_H_ */
