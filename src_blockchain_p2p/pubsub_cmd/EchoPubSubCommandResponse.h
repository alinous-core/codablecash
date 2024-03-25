/*
 * EchoPubSubCommandResponse.h
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#ifndef PUBSUB_ECHOPUBSUBCOMMANDRESPONSE_H_
#define PUBSUB_ECHOPUBSUBCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class EchoPubSubCommandResponse : public AbstractCommandResponse {
public:
	explicit EchoPubSubCommandResponse();
	virtual ~EchoPubSubCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	void setMessage(UnicodeString* str) noexcept;
	const UnicodeString* getMessage() const noexcept {
		return message;
	}

	virtual UnicodeString* toString() const noexcept;

private:
	UnicodeString* message;

};

} /* namespace codablecash */

#endif /* PUBSUB_ECHOPUBSUBCOMMANDRESPONSE_H_ */
