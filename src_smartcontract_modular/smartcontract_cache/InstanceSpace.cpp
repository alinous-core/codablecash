/*
 * InstanceSpace.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_cache/InstanceSpace.h"

namespace codablecash {

InstanceSpace::InstanceSpace() {
	this->instance = nullptr;

	// TODO Auto-generated constructor stub

}

InstanceSpace::~InstanceSpace() {
	delete this->instance;
}

} /* namespace codablecash */
