/*
 * BlockVersion.h
 *
 *  Created on: May 5, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BLOCKVERSION_H_
#define BC_BLOCK_BLOCKVERSION_H_

#include "bc/SoftwareVersion.h"

namespace codablecash {

class BlockVersion : public SoftwareVersion {
public:
	BlockVersion(const BlockVersion& inst);
	BlockVersion(int major, int minor, int patch);
	virtual ~BlockVersion();

	static BlockVersion* createFromBinary(ByteBuffer* in);
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BLOCKVERSION_H_ */
