/*
 * TransactionDataFactory.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionIdKeyFactory.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

TransactionIdKeyFactory::TransactionIdKeyFactory() {

}

TransactionIdKeyFactory::~TransactionIdKeyFactory() {

}

AbstractBtreeKey* TransactionIdKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == TransactionIdKeyFactory::TRANSACTION_ID_KEY){
		AbstractBtreeKey* key = TransactionIdKey::fromBinary(in);
		return key;
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* TransactionIdKeyFactory::copy() const noexcept {
	return new TransactionIdKeyFactory();
}

} /* namespace codablecash */
