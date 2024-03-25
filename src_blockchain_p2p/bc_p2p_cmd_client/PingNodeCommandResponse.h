/*
 * PingNodeCommandResponse.h
 *
 *  Created on: 2023/09/10
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_PINGNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_PINGNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class PingNodeCommandResponse : public AbstractCommandResponse {
public:
	PingNodeCommandResponse();
	virtual ~PingNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_PINGNODECOMMANDRESPONSE_H_ */
