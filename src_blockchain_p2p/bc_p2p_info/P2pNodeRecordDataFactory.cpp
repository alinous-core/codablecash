/*
 * P2pNodeRecordDataFactory.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p_info/P2pNodeRecordDataFactory.h"
#include "bc_p2p_info/P2pNodeRecord.h"

namespace codablecash {

P2pNodeRecordDataFactory::P2pNodeRecordDataFactory() {

}

P2pNodeRecordDataFactory::~P2pNodeRecordDataFactory() {

}

IBlockObject* P2pNodeRecordDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return P2pNodeRecord::fromBinary(in);
}

AbstractBtreeDataFactory* P2pNodeRecordDataFactory::copy() const noexcept {
	return new P2pNodeRecordDataFactory();
}

} /* namespace codablecash */
