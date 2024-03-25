/*
 * SignatureErrorResponse.h
 *
 *  Created on: 2023/04/06
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_SIGNATUREERRORRESPONSE_H_
#define BC_P2P_CMD_SIGNATUREERRORRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class SignatureErrorResponse : public AbstractCommandResponse {
public:
	SignatureErrorResponse();
	virtual ~SignatureErrorResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_SIGNATUREERRORRESPONSE_H_ */
