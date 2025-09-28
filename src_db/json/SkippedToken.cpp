/*
 * SkippedToken.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#include "json/SkippedToken.h"

namespace codablecash {

SkippedToken::SkippedToken(int line, int pos) : AbstractJsonToken(line, pos) {
	this->type = TokenType::NONE;
}

SkippedToken::~SkippedToken() {

}

} /* namespace codablecash */
