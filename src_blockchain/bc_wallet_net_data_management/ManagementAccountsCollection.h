/*
 * ManagementAccountsCollection.h
 *
 *  Created on: Jul 20, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_
#define BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_

#include "base/ArrayList.h"
#include <cstdint>

using namespace alinous;

namespace codablecash {

class ManagementAccount;
class TransactionId;

class ManagementAccountsCollection {
public:
	static constexpr const int POS_FINALIZED{0};
	static constexpr const int POS_UNFINALIZED{1};
	static constexpr const int POS_MEMPOOL{2};

	static int RECORD_SOTRE_TYPE_TO_INXED(uint8_t storeType) {
		return storeType - 1;
	}


	ManagementAccountsCollection();
	virtual ~ManagementAccountsCollection();

	void resetAll() noexcept;

	void resetFinalized() noexcept;
	void resetUnfinalized() noexcept;
	void resetMempool() noexcept;

	ManagementAccount* getFinalizedManagementAccount()const noexcept;
	ManagementAccount* getUnFinalizedManagementAccount()const noexcept;
	ManagementAccount* getMempoolManagementAccount()const noexcept;

	ManagementAccount* getManagementAccount(int pos) const noexcept;

	uint8_t getTransactionStoreStatus(const TransactionId* trxId) const noexcept;

private:
	ArrayList<ManagementAccount>* list;

};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_ */
