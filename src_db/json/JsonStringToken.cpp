/*
 * JsonStringToken.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#include "json/JsonStringToken.h"

namespace codablecash {

JsonStringToken::JsonStringToken(int line, int pos) : AbstractJsonToken(line, pos) {
	this->type = TokenType::STRING;
}

JsonStringToken::~JsonStringToken() {

}

} /* namespace codablecash */
