/*
 * LongRange.h
 *
 *  Created on: 2018/05/18
 *      Author: iizuka
 */

#ifndef FILESTORE_LONGRANGE_H_
#define FILESTORE_LONGRANGE_H_

#include <cstdint>

namespace alinous {

class ByteBuffer;

class LongRange {
public:
	LongRange(const LongRange& base) = delete;
	explicit LongRange(const LongRange* base) noexcept;
	LongRange(uint64_t min, uint64_t max) noexcept;
	virtual ~LongRange();

	int compare(uint64_t value) const noexcept;

	uint64_t width() const noexcept ;
	bool hasNext(uint64_t value) const noexcept ;
	uint64_t getMin() const noexcept ;
	void setMin(uint64_t min) noexcept ;
	uint64_t getMax() const noexcept ;
	void setMax(uint64_t max) noexcept ;

	bool removeLow(uint64_t value) noexcept;
	bool removeHigh(uint64_t value) noexcept;

	bool equals(const LongRange* other) noexcept;

	int binarySize() noexcept;
	void toBinary(ByteBuffer* buff) noexcept;
	static LongRange* fromBinary(ByteBuffer* buff) noexcept;

private:
	uint64_t min;
	uint64_t max;
};

} /* namespace alinous */

#endif /* FILESTORE_LONGRANGE_H_ */
