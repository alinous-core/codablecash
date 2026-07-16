/*
 * BlockchainSoftwareVersion.h
 *
 *  Created on: May 5, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCKCHAINSOFTWAREVERSION_H_
#define BC_BLOCKCHAINSOFTWAREVERSION_H_

#include "bc/SoftwareVersion.h"

namespace codablecash {

class BlockchainSoftwareVersion : public SoftwareVersion {
public:
	//BlockchainSoftwareVersion(const BlockchainSoftwareVersion& inst);
	BlockchainSoftwareVersion(int major, int minor, int patch);
	virtual ~BlockchainSoftwareVersion();

	//static BlockchainSoftwareVersion* createFromBinary(ByteBuffer* in);
};

} /* namespace codablecash */

#endif /* BC_BLOCKCHAINSOFTWAREVERSION_H_ */
