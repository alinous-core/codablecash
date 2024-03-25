/*
 * IValueProvider.cpp
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_condition/params/NullScanParam.h"
#include "scan_select/scan_condition/params/InfinityScanParam.h"

namespace codablecash {

const NullScanParam IValueProvider::NULLPARAM;
const InfinityScanParam IValueProvider::INFINITY_PARAM;

IValueProvider::IValueProvider() {

}

IValueProvider::~IValueProvider() {

}

bool IValueProvider::isColumn() const noexcept {
	return false;
}

bool codablecash::IValueProvider::hasColumnId() const noexcept {
	return false;
}


} /* namespace codablecash */
