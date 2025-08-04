/*
 * ManagementAccountsCollection.h
 *
 *  Created on: Jul 20, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_
#define BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class ManagementAccount;

class ManagementAccountsCollection {
public:
	static constexpr const int POS_FINALIZED{0};
	static constexpr const int POS_UNFINALIZED{1};
	static constexpr const int POS_MEMPOOL{2};

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

private:
	ArrayList<ManagementAccount>* list;

};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_MANAGEMENT_MANAGEMENTACCOUNTSCOLLECTION_H_ */
