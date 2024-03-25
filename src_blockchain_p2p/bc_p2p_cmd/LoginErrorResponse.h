/*
 * LoginErrorResponse.h
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_LOGINERRORRESPONSE_H_
#define BC_P2P_CMD_LOGINERRORRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class LoginErrorResponse : public AbstractCommandResponse {
public:
	LoginErrorResponse();
	virtual ~LoginErrorResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_LOGINERRORRESPONSE_H_ */
