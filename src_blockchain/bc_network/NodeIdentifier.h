/*
 * NodeIdentifier.h
 *
 *  Created on: 2019/01/25
 *      Author: iizuka
 */

#ifndef BC_NETWORK_NODEIDENTIFIER_H_
#define BC_NETWORK_NODEIDENTIFIER_H_
#include "filestore_block/IBlockObject.h"
#include <cstdint>

namespace alinous {
class ByteBuffer;
class BigInteger;
}

namespace codablecash {
using namespace alinous;

class NodeIdentifier : public IBlockObject {
public:
	//NodeIdentifier& operator= (NodeIdentifier&& inst);
	//NodeIdentifier(NodeIdentifier&& inst);
	NodeIdentifier(const NodeIdentifier& inst);

	NodeIdentifier();
	NodeIdentifier(const BigInteger* pubkey);
	virtual ~NodeIdentifier();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static NodeIdentifier* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	int compareTo(const NodeIdentifier* other) const noexcept;
	bool equals(const NodeIdentifier* other) const noexcept;

	const BigInteger* getPublicKey() const noexcept {
		return this->nodeIdentifier;
	}

	int hashCode() const;

private:
	BigInteger* nodeIdentifier; // pubkey

public:
	class ValueCompare {
	public:
		int operator () (const NodeIdentifier* const a, const NodeIdentifier* const b) const;
	};
};

} /* namespace codablecash */

#endif /* BC_NETWORK_NODEIDENTIFIER_H_ */
