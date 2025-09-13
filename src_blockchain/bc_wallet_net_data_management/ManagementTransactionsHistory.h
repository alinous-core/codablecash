/*
 * ManagementTransactionsHistory.h
 *
 *  Created on: Jul 17, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTTRANSACTIONSHISTORY_H_
#define BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTTRANSACTIONSHISTORY_H_

#include "base/ArrayList.h"
#include "base/HashMap.h"

using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;
class TransactionId;

class ManagementTransactionsHistory {
public:
	ManagementTransactionsHistory();
	virtual ~ManagementTransactionsHistory();

	void reset() noexcept;
	void addTransaction(const AbstractBlockchainTransaction* trx);
	const AbstractBlockchainTransaction* getTransaction(const TransactionId *trxId) const noexcept;
	bool hasTransaction(const TransactionId *trxId) const noexcept;

	void importOtherManagementTransactionsHistory(const ManagementTransactionsHistory* other);

	int historySize() const noexcept;
	const AbstractBlockchainTransaction* getTransaction(int i) const noexcept;


private:
	ArrayList<AbstractBlockchainTransaction>* list;
	HashMap<TransactionId, AbstractBlockchainTransaction>* map;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTTRANSACTIONSHISTORY_H_ */
