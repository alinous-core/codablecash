/*
 * AbstractJsonContainer.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_ABSTRACTJSONCONTAINER_H_
#define JSON_OBJECT_ABSTRACTJSONCONTAINER_H_

#include "json_object/AbstractJsonObject.h"

namespace codablecash {

class AbstractJsonContainer : public AbstractJsonObject {
public:
	AbstractJsonContainer();
	virtual ~AbstractJsonContainer();
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_ABSTRACTJSONCONTAINER_H_ */
