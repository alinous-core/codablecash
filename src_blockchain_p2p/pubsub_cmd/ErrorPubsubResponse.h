/*
 * ErrorPubsubResponse.h
 *
 *  Created on: 2023/09/13
 *      Author: iizuka
 */

#ifndef PUBSUB_CMD_ERRORPUBSUBRESPONSE_H_
#define PUBSUB_CMD_ERRORPUBSUBRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class ErrorPubsubResponse : public AbstractCommandResponse {
public:
	ErrorPubsubResponse();
	virtual ~ErrorPubsubResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void setMessage(const UnicodeString* message) noexcept;

private:
	UnicodeString* message;
};

} /* namespace codablecash */

#endif /* PUBSUB_CMD_ERRORPUBSUBRESPONSE_H_ */
