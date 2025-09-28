/*
 * JsonToken.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONTOKEN_H_
#define JSON_JSONTOKEN_H_

#include "json/AbstractJsonToken.h"

namespace codablecash {

class JsonToken : public AbstractJsonToken {
public:
	JsonToken(int line, int pos);
	virtual ~JsonToken();
};

} /* namespace codablecash */

#endif /* JSON_JSONTOKEN_H_ */
