/*
 * NodeIdentifierKeyFactory.h
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#ifndef BC_NETWORK_NODEIDENTIFIERKEYFACTORY_H_
#define BC_NETWORK_NODEIDENTIFIERKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifierKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t NODE_IDENTIFIER_KEY{0x14};

	NodeIdentifierKeyFactory();
	virtual ~NodeIdentifierKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_NODEIDENTIFIERKEYFACTORY_H_ */
