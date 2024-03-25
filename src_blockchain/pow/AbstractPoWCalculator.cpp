/*
 * AbstractPoWCalculator.cpp
 *
 *  Created on: 2023/03/22
 *      Author: iizuka
 */

#include "pow/AbstractPoWCalculator.h"

namespace codablecash {

AbstractPoWCalculator::AbstractPoWCalculator() {
	this->powManager = nullptr;
}

AbstractPoWCalculator::~AbstractPoWCalculator() {
	this->powManager = nullptr;
}

void AbstractPoWCalculator::setPowManager(PoWManager *powManager) noexcept {
	this->powManager = powManager;
}

} /* namespace codablecash */
