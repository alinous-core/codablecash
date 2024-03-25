/*
 * NodeIdentifierListDataFactory.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_network/NodeIdentifier.h"
#include "bc_p2p_info/NodeIdentifierDataFactory.h"

namespace codablecash {

NodeIdentifierDataFactory::NodeIdentifierDataFactory() {

}

NodeIdentifierDataFactory::~NodeIdentifierDataFactory() {

}

IBlockObject* NodeIdentifierDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return NodeIdentifier::fromBinary(in);
}

AbstractBtreeDataFactory* NodeIdentifierDataFactory::copy() const noexcept {
	return new NodeIdentifierDataFactory();
}

} /* namespace codablecash */
