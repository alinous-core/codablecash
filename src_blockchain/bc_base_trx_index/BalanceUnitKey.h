/*
 * BalanceUnitKey.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_BALANCEUNITKEY_H_
#define BC_BASE_TRX_INDEX_BALANCEUNITKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class BalanceUnit;
class TransactionId;

class BalanceUnitKey : public AbstractBtreeKey {
public:
	explicit BalanceUnitKey(const BalanceUnit* unit);
	virtual ~BalanceUnitKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BalanceUnitKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	const TransactionId* getTrxId() const noexcept {
		return this->trxId;
	}

	void setTransactionId(const TransactionId* trxId) noexcept;

private:
	BalanceUnit* unit;
	TransactionId* trxId; // on remove
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_BALANCEUNITKEY_H_ */
