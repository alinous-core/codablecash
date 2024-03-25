/*
 * BalanceUnitKeyFactory.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_base_trx_index/BalanceUnitKeyFactory.h"
#include "bc_base_trx_index/BalanceUnitKey.h"

namespace codablecash {

BalanceUnitKeyFactory::BalanceUnitKeyFactory() {

}

BalanceUnitKeyFactory::~BalanceUnitKeyFactory() {

}

AbstractBtreeKey* BalanceUnitKeyFactory::fromBinary(uint32_t keyType,	ByteBuffer *in) const {
	if(keyType == BalanceUnitKeyFactory::BALANCE_UNIT_KEY){
		return BalanceUnitKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* BalanceUnitKeyFactory::copy() const noexcept {
	return new BalanceUnitKeyFactory();
}

} /* namespace codablecash */
