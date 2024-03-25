/*
 * AstroBwt.h
 *
 *  Created on: 2022/04/12
 *      Author: iizuka
 */

#ifndef ASTROBWT_ASTROBWT_H_
#define ASTROBWT_ASTROBWT_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BwtTransform;

class AstroBwt {
public:
	static const constexpr uint32_t stage1_length{69371}; // it is a prime
	static const constexpr uint32_t stage2_max_length{1024*1024 + 69371 + 1024}; // it is a prime
	//static const constexpr uint32_t stage2_max_length{69371 + 0xFFFF * 2};

	static const constexpr uint32_t DEFAULT_MASK{0xfffff};
	static const constexpr uint32_t DEFAULT_MASK2{0xffff};

	AstroBwt();
	~AstroBwt();
	ByteBuffer * POW(const uint8_t* data, int length) const;

	void setMask(uint32_t m) noexcept {
		this->transformMask = m;
	}

private:
	uint8_t* stage1;
	uint8_t* stage2;
	ByteBuffer* bwtBuff;

	BwtTransform* bwtTransform;
	uint32_t transformMask;

};

} /* namespace codablecash */

#endif /* ASTROBWT_ASTROBWT_H_ */
