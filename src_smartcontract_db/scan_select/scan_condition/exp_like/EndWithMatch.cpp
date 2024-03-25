/*
 * EndWithMatch.cpp
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/EndWithMatch.h"

#include "base/UnicodeString.h"

namespace codablecash {

EndWithMatch::EndWithMatch(const UnicodeString* pattern) {
	this->pattern = new UnicodeString(pattern);
}

EndWithMatch::~EndWithMatch() {
	delete pattern;
}

int EndWithMatch::match(const UnicodeString* str, int start) {
	int idx = str->lastIndexOf(this->pattern, str->length());

	if(idx < start){
		return -1;
	}

	return idx + this->pattern->length();
}

} /* namespace codablecash */
