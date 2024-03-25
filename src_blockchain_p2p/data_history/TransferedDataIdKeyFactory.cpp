/*
 * TransferedDataIdKeyFactory.cpp
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#include "data_history/TransferedDataIdKeyFactory.h"
#include "data_history/TransferedDataIdKey.h"

namespace codablecash {

TransferedDataIdKeyFactory::TransferedDataIdKeyFactory() {

}

TransferedDataIdKeyFactory::~TransferedDataIdKeyFactory() {

}

AbstractBtreeKey* TransferedDataIdKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == TRANSFERED_DATA_ID_KEY){
		AbstractBtreeKey* key = TransferedDataIdKey::fromBinary(in);
		return key;
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* TransferedDataIdKeyFactory::copy() const noexcept {
	return new TransferedDataIdKeyFactory();
}

} /* namespace codablecash */
