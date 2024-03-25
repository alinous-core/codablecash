/*
 * TransferedDataFactory.cpp
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#include "data_history/TransferedDataFactory.h"
#include "data_history/AbstractTransferedData.h"

namespace codablecash {

TransferedDataFactory::TransferedDataFactory() {

}

TransferedDataFactory::~TransferedDataFactory() {

}

IBlockObject* TransferedDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return AbstractTransferedData::createFromBinary(in);
}

AbstractBtreeDataFactory* TransferedDataFactory::copy() const noexcept {
	return new TransferedDataFactory();
}

} /* namespace codablecash */
