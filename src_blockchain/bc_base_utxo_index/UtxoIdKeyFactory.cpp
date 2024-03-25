/*
 * UtxoIdKeyFactory.cpp
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#include "bc_base_utxo_index/UtxoIdKeyFactory.h"
#include "bc_base_utxo_index/UtxoIdKey.h"

namespace codablecash {

UtxoIdKeyFactory::UtxoIdKeyFactory() {

}

UtxoIdKeyFactory::~UtxoIdKeyFactory() {

}

AbstractBtreeKey* UtxoIdKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == UtxoIdKeyFactory::UTXO_ID_KEY){
		return UtxoIdKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* UtxoIdKeyFactory::copy() const noexcept {
	return new UtxoIdKeyFactory();
}

} /* namespace codablecash */
