/*
 * JsonNumericValue.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_JSONNUMERICVALUE_H_
#define JSON_OBJECT_JSONNUMERICVALUE_H_

#include "json_object/AbstractJsonValue.h"

namespace codablecash {

class JsonNumericValue : public AbstractJsonValue {
public:
	JsonNumericValue();
	virtual ~JsonNumericValue();

	virtual uint8_t getType() const noexcept {
		return JSON_TYPE_NUMERIC_VALUE;
	}

	void setValue(int v) {
		this->value = v;
	}
	int getValue() const noexcept {
		return this->value;
	}

private:
	int value;
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_JSONNUMERICVALUE_H_ */
