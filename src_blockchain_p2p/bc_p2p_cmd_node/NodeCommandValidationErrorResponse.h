/*
 * NodeCommandValidationErrorResponse.h
 *
 *  Created on: Aug 21, 2024
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_NODECOMMANDVALIDATIONERRORRESPONSE_H_
#define BC_P2P_CMD_NODE_NODECOMMANDVALIDATIONERRORRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class NodeCommandValidationErrorResponse : public AbstractCommandResponse {
public:
	NodeCommandValidationErrorResponse();
	virtual ~NodeCommandValidationErrorResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_NODECOMMANDVALIDATIONERRORRESPONSE_H_ */
