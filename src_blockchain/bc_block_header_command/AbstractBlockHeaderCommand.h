/*
 * AbstractBlockHeaderCommand.h
 *
 *  Created on: Jun 13, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCK_HEADER_COMMAND_ABSTRACTBLOCKHEADERCOMMAND_H_
#define BC_BLOCK_HEADER_COMMAND_ABSTRACTBLOCKHEADERCOMMAND_H_
#include <cstdint>

#include "filestore_block/IBlockObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class AbstractBlockHeaderCommand : public alinous::IBlockObject {
public:
	static constexpr const uint16_t NEW_SHARD_COMMAND = 1;

	AbstractBlockHeaderCommand(const AbstractBlockHeaderCommand& inst);
	explicit AbstractBlockHeaderCommand(uint16_t type);
	virtual ~AbstractBlockHeaderCommand();

	static AbstractBlockHeaderCommand* createFromBinary(ByteBuffer* in);

	virtual void fromBinary(ByteBuffer* in) = 0;

protected:
	uint16_t type;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_HEADER_COMMAND_ABSTRACTBLOCKHEADERCOMMAND_H_ */
