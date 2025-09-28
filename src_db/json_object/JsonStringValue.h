/*
 * JsonStringValue.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_JSONSTRINGVALUE_H_
#define JSON_OBJECT_JSONSTRINGVALUE_H_

#include "json_object/AbstractJsonValue.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;


namespace codablecash {

class JsonStringValue : public AbstractJsonValue {
public:
	JsonStringValue();
	virtual ~JsonStringValue();

	virtual uint8_t getType() const noexcept {
		return JSON_TYPE_STRING_VALUE;
	}

	void setValue(const UnicodeString* value) noexcept;

private:
	UnicodeString* value;
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_JSONSTRINGVALUE_H_ */
