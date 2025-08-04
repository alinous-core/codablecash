/*
 * ManagementAccountsCollection.cpp
 *
 *  Created on: Jul 20, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data_management/ManagementAccountsCollection.h"
#include "bc_wallet_net_data_management/ManagementAccount.h"

#include "bc_wallet_net_data_management/ManagedUtxoCacheRecord.h"
namespace codablecash {

ManagementAccountsCollection::ManagementAccountsCollection() {
	this->list = new ArrayList<ManagementAccount>();
	this->list->addElement(new ManagementAccount(ManagedUtxoCacheRecord::FINALIZED));
	this->list->addElement(new ManagementAccount(ManagedUtxoCacheRecord::UNFINALIZED));
	this->list->addElement(new ManagementAccount(ManagedUtxoCacheRecord::MEMPOOL));
}

ManagementAccountsCollection::~ManagementAccountsCollection() {
	this->list->deleteElements();
	delete this->list;
}

void ManagementAccountsCollection::resetAll() noexcept {
	resetFinalized();
	resetUnfinalized();
	resetMempool();
}

void ManagementAccountsCollection::resetFinalized() noexcept {
	ManagementAccount* account = this->list->get(POS_FINALIZED);
	account->reset();
}

void ManagementAccountsCollection::resetUnfinalized() noexcept {
	ManagementAccount* account = this->list->get(POS_UNFINALIZED);
	account->reset();
}

void ManagementAccountsCollection::resetMempool() noexcept {
	ManagementAccount* account = this->list->get(POS_MEMPOOL);
	account->reset();
}

ManagementAccount* ManagementAccountsCollection::getFinalizedManagementAccount() const noexcept {
	return getManagementAccount(POS_FINALIZED);
}

ManagementAccount* ManagementAccountsCollection::getUnFinalizedManagementAccount() const noexcept {
	return getManagementAccount(POS_UNFINALIZED);
}

ManagementAccount* ManagementAccountsCollection::getMempoolManagementAccount() const noexcept {
	return getManagementAccount(POS_MEMPOOL);
}

ManagementAccount* ManagementAccountsCollection::getManagementAccount(int pos) const noexcept {
	return this->list->get(pos);
}

} /* namespace codablecash */
