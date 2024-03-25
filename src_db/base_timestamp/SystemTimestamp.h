/*
 * SystemTimestamp.h
 *
 *  Created on: 2023/12/27
 *      Author: iizuka
 */

#ifndef BASE_TIMESTAMP_SYSTEMTIMESTAMP_H_
#define BASE_TIMESTAMP_SYSTEMTIMESTAMP_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

namespace alinous {

class BigInteger;

class SystemTimestamp : public IBlockObject {
public:
	static const constexpr uint32_t MAX_USEC{1000*1000};

	SystemTimestamp& operator=(const SystemTimestamp &o);
	SystemTimestamp operator+(const SystemTimestamp &o) const;
	SystemTimestamp operator-(const SystemTimestamp &o) const;

	SystemTimestamp(const SystemTimestamp& inst);
	SystemTimestamp();
	SystemTimestamp(uint64_t sec, uint32_t usec);

	virtual ~SystemTimestamp();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static SystemTimestamp* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	int compareTo(const SystemTimestamp* other) const noexcept;

	uint64_t getSec() const noexcept {
		return this->sec;
	}
	uint32_t getUsec() const noexcept {
		return this->usec;
	}

	BigInteger toBigIntegerMills() const noexcept;

	bool isZero() const noexcept;

private:
	void adjustUsec() noexcept;

private:
	uint64_t sec;
	uint32_t usec;
};

} /* namespace alinous */

#endif /* BASE_TIMESTAMP_SYSTEMTIMESTAMP_H_ */
