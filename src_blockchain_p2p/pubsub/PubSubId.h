/*
 * PubSubId.h
 *
 *  Created on: 2023/01/08
 *      Author: iizuka
 */

#ifndef PUBSUB_PUBSUBID_H_
#define PUBSUB_PUBSUBID_H_
#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class PubSubId {
public:
	PubSubId(const PubSubId& inst);
	PubSubId(const char* binary, int length);
	virtual ~PubSubId();

	static PubSubId* createNewId();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static PubSubId* fromBinary(ByteBuffer* in);

	bool equals(const PubSubId* other) const noexcept;

	int hashCode() const;
	class ValueCompare {
	public:
		int operator () (const PubSubId* const a, const PubSubId* const b) const;
	};

private:
	ByteBuffer* id; // 32 bytes

	static uint64_t serial;
};

} /* namespace codablecash */

#endif /* PUBSUB_PUBSUBID_H_ */
