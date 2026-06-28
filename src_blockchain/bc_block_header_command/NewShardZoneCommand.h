/*
 * NewShardZoneCommand.h
 *
 *  Created on: Jun 13, 2026
 *      Author: iizuka
 */

#ifndef BC_BLOCK_HEADER_COMMAND_NEWSHARDZONECOMMAND_H_
#define BC_BLOCK_HEADER_COMMAND_NEWSHARDZONECOMMAND_H_

#include "bc_block_header_command/AbstractBlockHeaderCommand.h"

namespace codablecash {

class NewShardZoneCommand : public AbstractBlockHeaderCommand {
public:
	NewShardZoneCommand(const NewShardZoneCommand& inst);
	NewShardZoneCommand();
	virtual ~NewShardZoneCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setNewShardNo(uint16_t newShardNo) noexcept;

private:
	uint16_t newShardNo;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_HEADER_COMMAND_NEWSHARDZONECOMMAND_H_ */
