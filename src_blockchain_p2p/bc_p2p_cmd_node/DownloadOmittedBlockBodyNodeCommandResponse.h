/*
 * DownloadOmittedBlockBodyNodeCommandResponse.h
 *
 *  Created on: 2023/11/24
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADOMITTEDBLOCKBODYNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_NODE_DOWNLOADOMITTEDBLOCKBODYNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class OmittedBlockBody;

class DownloadOmittedBlockBodyNodeCommandResponse : public AbstractCommandResponse {
public:
	DownloadOmittedBlockBodyNodeCommandResponse();
	virtual ~DownloadOmittedBlockBodyNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void setOmittedBlockBody(const OmittedBlockBody* body);
	const OmittedBlockBody* getBody() const noexcept {
		return this->body;
	}

private:
	OmittedBlockBody* body;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADOMITTEDBLOCKBODYNODECOMMANDRESPONSE_H_ */
