/*
 * DownloadBlockNodeCommandResponse.h
 *
 *  Created on: 2023/11/18
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class Block;

class DownloadBlockNodeCommandResponse : public AbstractCommandResponse {
public:
	//DownloadBlockNodeCommandResponse(const DownloadBlockNodeCommandResponse& inst);
	DownloadBlockNodeCommandResponse();
	virtual ~DownloadBlockNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void setBlock(const Block* block);
	Block* getBlock() const noexcept {
		return this->block;
	}

private:
	Block* block;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADBLOCKNODECOMMANDRESPONSE_H_ */
