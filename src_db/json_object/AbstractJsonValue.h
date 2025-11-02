/*
 * AbstractJsonValue.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_ABSTRACTJSONVALUE_H_
#define JSON_OBJECT_ABSTRACTJSONVALUE_H_

#include "json_object/AbstractJsonObject.h"

namespace codablecash {

class AbstractJsonValue : public AbstractJsonObject {
public:
	AbstractJsonValue();
	virtual ~AbstractJsonValue();
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_ABSTRACTJSONVALUE_H_ */
