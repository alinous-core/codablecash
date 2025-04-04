/*
 * ReceivingAddress.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/ReceivingAddressStore.h"

#include "bc_base_conf_store/StatusStore.h"


namespace codablecash {

ReceivingAddressStore::ReceivingAddressStore(uint16_t zone, int maxAddress, const File* baseDir)
		: AbstractAddressStore(zone, baseDir, ReceivingAddressStore::STORE_NAME) {
	this->maxAddress = maxAddress;
}

ReceivingAddressStore::~ReceivingAddressStore() {

}

void ReceivingAddressStore::init(const IWalletDataEncoder *encoder) {
	int maxLoop = this->maxAddress;
	for(int i = 0; i != maxLoop; ++i){
		AddressAndPrivateKey* pair = createNewAddressAndPrivateKey(encoder, this->adressSerial++);

		this->list->addElement(pair);
	}

	save();
}

void ReceivingAddressStore::save() {
	__save();

	this->store->addShortValue(KEY_MAX_ADDRESS, this->maxAddress);
}

void ReceivingAddressStore::load(const IWalletDataEncoder *encoder) {
	this->store->load();
	__load();

	this->maxAddress = this->store->getShortValue(KEY_MAX_ADDRESS);

	for(int i = 0; i != this->adressSerial; ++i){
		AddressAndPrivateKey* pair = createNewAddressAndPrivateKey(encoder, i);

		this->list->addElement(pair);
	}
}

} /* namespace codablecash */
