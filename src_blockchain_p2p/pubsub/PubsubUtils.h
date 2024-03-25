/*
 * PubsubUtils.h
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#ifndef PUBSUB_PUBSUBUTILS_H_
#define PUBSUB_PUBSUBUTILS_H_

namespace alinous {
class ByteBuffer;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class IClientSocket;

class PubsubUtils {
public:
	static void putString(ByteBuffer* out, const UnicodeString* str) noexcept;
	static UnicodeString* getString(ByteBuffer* in) noexcept;
	static int stringSize(const UnicodeString* str) noexcept;

	static ByteBuffer* readAll(IClientSocket *client, int size);
};

} /* namespace codablecash */

#endif /* PUBSUB_PUBSUBUTILS_H_ */
