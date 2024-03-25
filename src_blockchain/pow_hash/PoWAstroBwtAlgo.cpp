/*
 * PoWAstroBwtAlgo.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PoWAstroBwtAlgo.h"

#include "astrobwt/AstroBwt.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "osenv/memory.h"
using alinous::Mem;


namespace codablecash {

PoWAstroBwtAlgo::PoWAstroBwtAlgo(const AstroBwt* astroBwt)
		: AbstractHashAlgorithmElement() {
	this->astroBwt = astroBwt;
}

PoWAstroBwtAlgo::~PoWAstroBwtAlgo() {
	this->astroBwt = nullptr;
}

void PoWAstroBwtAlgo::calulate(const char *in01, const char *in02, const char *in03, char *out) const noexcept {
	ByteBuffer* buff = this->astroBwt->POW((const uint8_t*)in01, 32); __STP(buff);

	buff->position(0);
	Mem::memcpy(out, buff->array(), 32);
}

} /* namespace codablecash */
