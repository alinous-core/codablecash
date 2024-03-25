/*
 * ChangeAddressStore.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/ChangeAddressStore.h"

#include "bc_base_conf_store/StatusStore.h"

namespace codablecash {

ChangeAddressStore::ChangeAddressStore(uint16_t zone, int numgroup, const File* baseDir)
		: AbstractAddressStore(zone, baseDir, ChangeAddressStore::STORE_NAME) {
	this->currentIndex = 0;
	this->numgroup = numgroup;
}

ChangeAddressStore::~ChangeAddressStore() {

}

void ChangeAddressStore::init(const IWalletDataEncoder *encoder) {
	int maxLoop = this->numgroup;
	for(int i = 0; i != maxLoop; ++i){
		AddressAndPrivateKey* pair = createNewAddressAndPrivateKey(encoder, this->adressSerial++);

		this->list->addElement(pair);
	}

	save();
}

AddressDescriptor* ChangeAddressStore::getNextChangeAddress(const IWalletDataEncoder *encoder) noexcept {
	if(this->currentIndex >= this->adressSerial){
		AddressAndPrivateKey* pair = createNewAddressAndPrivateKey(encoder, this->adressSerial++);
		this->list->addElement(pair);
		save();
	}
	return getAddressDescriptor(this->currentIndex++);
}

void ChangeAddressStore::save() {
	__save();

	this->store->addLongValue(KEY_CURRENT_INDEX, this->currentIndex);
	this->store->addLongValue(KEY_NUM_GROUP, this->numgroup);
}

void ChangeAddressStore::load(const IWalletDataEncoder *encoder) {
	this->store->load();
	__load();

	this->currentIndex = this->store->getLongValue(KEY_CURRENT_INDEX);
	this->numgroup = this->store->getLongValue(KEY_NUM_GROUP);

	for(int i = 0; i != this->adressSerial; ++i){
		AddressAndPrivateKey* pair = createNewAddressAndPrivateKey(encoder, i);

		this->list->addElement(pair);
	}
}

} /* namespace codablecash */
