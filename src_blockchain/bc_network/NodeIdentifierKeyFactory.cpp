/*
 * NodeIdentifierKeyFactory.cpp
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifierKeyFactory.h"
#include "bc_network/NodeIdentifierKey.h"


namespace codablecash {

NodeIdentifierKeyFactory::NodeIdentifierKeyFactory() {

}

NodeIdentifierKeyFactory::~NodeIdentifierKeyFactory() {

}

AbstractBtreeKey* NodeIdentifierKeyFactory::fromBinary(uint32_t keyType,	ByteBuffer *in) const {
	if(keyType == NODE_IDENTIFIER_KEY){
		return NodeIdentifierKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* NodeIdentifierKeyFactory::copy() const noexcept {
	return new NodeIdentifierKeyFactory();
}

} /* namespace codablecash */
