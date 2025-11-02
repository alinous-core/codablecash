/*
 * BloomHash512.h
 *
 *  Created on: May 6, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_FILTER_BLOOMHASH1024_H_
#define BC_WALLET_FILTER_BLOOMHASH1024_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BloomHash1024 {
public:
	static constexpr const uint16_t FILTER8{0xFF};
	static constexpr const int NUM_EXTRA_BITS{5};
	static constexpr const uint16_t FILTER2{0x1F};

	BloomHash1024(const BloomHash1024& inst);
	BloomHash1024(uint8_t v0, uint8_t v1, uint8_t v2, uint16_t flag);
	BloomHash1024(uint16_t v0, uint16_t v1, uint16_t v2);
	virtual ~BloomHash1024();

	uint16_t getValue(int pos) const noexcept;

	int binarySize() const;
	void toBinary(ByteBuffer* out);
	static BloomHash1024* createFromBinary(ByteBuffer* in);

	bool equals(const BloomHash1024* other) const noexcept;

	BloomHash1024* copyData() const noexcept;

private:
	void setBits(uint16_t base, int pos) noexcept;
	uint16_t getUpperBits(int pos) const noexcept;

private:
	uint8_t values[3]{};
	uint16_t flag;
};

} /* namespace codablecash */

#endif /* BC_WALLET_FILTER_BLOOMHASH1024_H_ */
