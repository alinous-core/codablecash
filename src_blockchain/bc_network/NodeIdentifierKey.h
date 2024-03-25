/*
 * NodeIdentifierKey.h
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#ifndef BC_NETWORK_NODEIDENTIFIERKEY_H_
#define BC_NETWORK_NODEIDENTIFIERKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifier;

class NodeIdentifierKey : public AbstractBtreeKey {
public:
	NodeIdentifierKey(const NodeIdentifier* nodeId);
	virtual ~NodeIdentifierKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static NodeIdentifierKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

private:
	NodeIdentifier* nodeId;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_NODEIDENTIFIERKEY_H_ */
