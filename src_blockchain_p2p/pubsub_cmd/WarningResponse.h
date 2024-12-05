/*
 * WarningResponse.h
 *
 *  Created on: Sep 23, 2024
 *      Author: iizuka
 */

#ifndef PUBSUB_CMD_WARNINGRESPONSE_H_
#define PUBSUB_CMD_WARNINGRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class WarningResponse : public AbstractCommandResponse {
public:
	WarningResponse();
	virtual ~WarningResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void setMessage(const UnicodeString* str) noexcept;

private:
	UnicodeString* message;
};

}

#endif /* PUBSUB_CMD_WARNINGRESPONSE_H_ */
