/*
 * AbstractJsonToken.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#include "json/AbstractJsonToken.h"

#include "base/UnicodeString.h"

namespace codablecash {

AbstractJsonToken::AbstractJsonToken(int line, int pos) {
	this->line = line;
	this->pos = pos;
	this->body = new UnicodeString(L"");
	this->type = NONE;
}

AbstractJsonToken::~AbstractJsonToken() {
	delete this->body;
}

void AbstractJsonToken::addChar(wchar_t ch) noexcept {
	this->body->append(ch);
}

void AbstractJsonToken::setType(TokenType type) {
	this->type = type;
}

} /* namespace codablecash */
