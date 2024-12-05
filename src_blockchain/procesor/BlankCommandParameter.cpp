/*
 * BlankCommandParameter.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "procesor/BlankCommandParameter.h"

#include "base_thread/SysMutex.h"

#include "base_thread/StackUnlocker.h"
namespace codablecash {

BlankCommandParameter::BlankCommandParameter() {
	this->mutex = new SysMutex();
	this->count = 0;
}

BlankCommandParameter::~BlankCommandParameter() {
	delete this->mutex;
}

void BlankCommandParameter::incCount() noexcept {
	StackUnlocker unlocker(this->mutex, __FILE__, __LINE__);
	this->count++;
}

} /* namespace codablecash */
