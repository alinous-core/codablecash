/*
 * InstanceIdIndexDataFactory.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor_index/InstanceIdIndexDataFactory.h"
#include "smartcontract_executor_index/InstanceIdIndexData.h"


namespace codablecash {

InstanceIdIndexDataFactory::InstanceIdIndexDataFactory() {


}

InstanceIdIndexDataFactory::~InstanceIdIndexDataFactory() {

}

IBlockObject* InstanceIdIndexDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return InstanceIdIndexData::createFromBinary(in);
}

AbstractBtreeDataFactory* InstanceIdIndexDataFactory::copy() const noexcept {
	return new InstanceIdIndexDataFactory();
}

} /* namespace codablecash */
