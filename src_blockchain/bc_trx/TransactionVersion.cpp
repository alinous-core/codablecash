/*
 * TransactionVersion.cpp
 *
 *  Created on: May 6, 2026
 *      Author: iizuka
 */

#include "bc_trx/TransactionVersion.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

TransactionVersion::TransactionVersion(const TransactionVersion &inst) : SoftwareVersion(inst.major, inst.minor, inst.patch) {

}

TransactionVersion::TransactionVersion(int major, int minor, int patch) : SoftwareVersion(major, minor, patch) {

}

TransactionVersion::~TransactionVersion() {

}

TransactionVersion* TransactionVersion::createFromBinary(ByteBuffer *in) {
	uint8_t major = in->get();
	uint8_t minor = in->get();
	uint8_t pathch = in->get();

	return new TransactionVersion(major, minor, pathch);
}

} /* namespace codablecash */
