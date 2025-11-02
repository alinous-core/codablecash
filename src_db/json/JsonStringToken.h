/*
 * JsonStringToken.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONSTRINGTOKEN_H_
#define JSON_JSONSTRINGTOKEN_H_

#include "json/AbstractJsonToken.h"

namespace codablecash {

class JsonStringToken : public AbstractJsonToken {
public:
	JsonStringToken(int line, int pos);
	virtual ~JsonStringToken();
};

} /* namespace codablecash */

#endif /* JSON_JSONSTRINGTOKEN_H_ */
