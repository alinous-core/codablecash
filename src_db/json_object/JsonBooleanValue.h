/*
 * JsonBooleanValue.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_JSONBOOLEANVALUE_H_
#define JSON_OBJECT_JSONBOOLEANVALUE_H_

#include "json_object/AbstractJsonValue.h"

namespace codablecash {

class JsonBooleanValue : public AbstractJsonValue {
public:
	JsonBooleanValue();
	virtual ~JsonBooleanValue();

	virtual uint8_t getType() const noexcept {
		return JSON_TYPE_BOOLEAN_VALUE;
	}

	bool isValue() const {
		return value;
	}

	void setValue(bool value) {
		this->value = value;
	}

private:
	bool value;
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_JSONBOOLEANVALUE_H_ */
