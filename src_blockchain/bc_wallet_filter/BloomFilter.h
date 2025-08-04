/*
 * BloomFilter.h
 *
 *  Created on: Mar 28, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_FILTER_BLOOMFILTER_H_
#define BC_WALLET_FILTER_BLOOMFILTER_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BloomFilter {
public:
	BloomFilter(const BloomFilter& inst);
	explicit BloomFilter(uint16_t byteLength);
	virtual ~BloomFilter();

	virtual void add(const char* b, int length);

	void setBit(int pos) noexcept;
	bool hasBit(int pos);

	bool checkBytes(const char* b, int length);
	bool checkBytes(uint64_t v1, uint64_t v2, uint64_t v3);

	bool equals(const BloomFilter* other) const noexcept;

private:
	bool filterBinaryEquals(const BloomFilter* other) const noexcept;

protected:


	int __binarySize() const;
	void __toBinary(ByteBuffer* out) const;
	void __fromBinary(ByteBuffer* in);

protected:
	uint16_t byteLength;
	uint8_t* buffer;

	int bitlength;
};

} /* namespace codablecash */

#endif /* BC_WALLET_FILTER_BLOOMFILTER_H_ */
