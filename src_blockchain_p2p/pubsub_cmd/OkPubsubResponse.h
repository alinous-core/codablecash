/*
 * OkPubsubResponse.h
 *
 *  Created on: 2023/01/15
 *      Author: iizuka
 */

#ifndef PUBSUB_CMD_OKPUBSUBRESPONSE_H_
#define PUBSUB_CMD_OKPUBSUBRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class OkPubsubResponse : public AbstractCommandResponse {
public:
	OkPubsubResponse();
	virtual ~OkPubsubResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;
};

} /* namespace codablecash */

#endif /* PUBSUB_CMD_OKPUBSUBRESPONSE_H_ */
