/*
 * CompleteMatch.cpp
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/CompleteMatch.h"

#include "base/UnicodeString.h"


namespace codablecash {

CompleteMatch::CompleteMatch(const UnicodeString* pattern) {
	this->pattern = new UnicodeString(pattern);
}

CompleteMatch::~CompleteMatch() {
	delete this->pattern;
}

int CompleteMatch::match(const UnicodeString* str, int start) {
	int idx = str->indexOf(this->pattern, start);
	int idxEnds = idx + this->pattern->length();

	if(idxEnds != str->length()){
		return -1;
	}

	return idxEnds;
}

} /* namespace codablecash */
