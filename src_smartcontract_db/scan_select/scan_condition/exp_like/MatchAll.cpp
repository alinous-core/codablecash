/*
 * MatchAll.cpp
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/MatchAll.h"

#include "base/UnicodeString.h"

namespace codablecash {

MatchAll::MatchAll() {

}

MatchAll::~MatchAll() {

}

int MatchAll::match(const UnicodeString* str, int start) {
	return str->length();
}

} /* namespace codablecash */
