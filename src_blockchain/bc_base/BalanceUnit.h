/*
 * BalanceUnit.h
 *
 *  Created on: 2019/01/03
 *      Author: iizuka
 */

#ifndef BC_BASE_BALANCEUNIT_H_
#define BC_BASE_BALANCEUNIT_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

namespace codablecash {
using namespace alinous;

class BalanceUnit : public IBlockObject {
public:
	static const constexpr uint8_t TYPE_PERICA{1};

	BalanceUnit &operator=(const BalanceUnit &o);
	BalanceUnit operator+(const BalanceUnit &o) const;
	BalanceUnit operator-(const BalanceUnit &o) const;
	BalanceUnit operator*(const BalanceUnit &o) const;
	BalanceUnit operator/(const uint64_t div) const;

	BalanceUnit& operator += ( const BalanceUnit& b );
	BalanceUnit& operator -= ( const BalanceUnit& b );

	uint8_t getType() const noexcept {
		return TYPE_PERICA;
	}

	BalanceUnit(const BalanceUnit& inst);
	BalanceUnit();
	explicit BalanceUnit(uint64_t amount);
	virtual ~BalanceUnit();

	uint64_t getAmount() const noexcept;
	void setAmount(uint64_t amount) noexcept{this->amount = amount;}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;

	static BalanceUnit* fromBinary(ByteBuffer* in);
	void importBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	int compareTo(const BalanceUnit& other) const noexcept;
	int compareTo(const BalanceUnit* other) const noexcept;
	bool isZero() const noexcept {
		return this->amount == 0;
	}

private:
	uint8_t type;
	uint64_t amount;
};

} /* namespace codablecash */

#endif /* BC_BASE_BALANCEUNIT_H_ */
