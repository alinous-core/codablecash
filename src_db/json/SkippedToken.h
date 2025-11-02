/*
 * SkippedToken.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_SKIPPEDTOKEN_H_
#define JSON_SKIPPEDTOKEN_H_

#include "json/AbstractJsonToken.h"

namespace codablecash {

class SkippedToken : public AbstractJsonToken {
public:
	SkippedToken(int line, int pos);
	virtual ~SkippedToken();
};

} /* namespace codablecash */

#endif /* JSON_SKIPPEDTOKEN_H_ */
