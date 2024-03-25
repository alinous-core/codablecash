/*
 * BeginWithMatch.cpp
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/BeginWithMatch.h"

#include "base/UnicodeString.h"


namespace codablecash {

BeginWithMatch::BeginWithMatch(const UnicodeString* pattern) {
	this->pattern = new UnicodeString(pattern);
}

BeginWithMatch::~BeginWithMatch() {
	delete this->pattern;
}

int BeginWithMatch::match(const UnicodeString* str, int start) {
	if(str->startsWith(this->pattern, start)){
		return start + this->pattern->length();
	}

	return -1;
}

} /* namespace codablecash */
