/*
 * BalanceUnitKeyFactory.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_BALANCEUNITKEYFACTORY_H_
#define BC_BASE_TRX_INDEX_BALANCEUNITKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class BalanceUnitKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t BALANCE_UNIT_KEY{0x14};

	BalanceUnitKeyFactory();
	virtual ~BalanceUnitKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_BALANCEUNITKEYFACTORY_H_ */
