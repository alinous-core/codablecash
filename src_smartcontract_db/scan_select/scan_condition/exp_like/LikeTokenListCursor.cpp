/*
 * LikeTokenListCursor.cpp
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/LikeTokenListCursor.h"

#include "base/ArrayList.h"
#include "base/UnicodeString.h"

namespace codablecash {

LikeTokenListCursor::LikeTokenListCursor(const ArrayList<UnicodeString>* list) {
	this->list = list;
	this->pos = 0;
}

LikeTokenListCursor::~LikeTokenListCursor() {

}

const UnicodeString* LikeTokenListCursor::next() noexcept {
	return this->list->get(this->pos++);
}

const UnicodeString* LikeTokenListCursor::lookAhead() const noexcept {
	int nextPos = this->pos;

	return this->list->size() > nextPos ? this->list->get(nextPos) : nullptr;
}

} /* namespace codablecash */
