/*
 * JsonArrayObject.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_JSONARRAYOBJECT_H_
#define JSON_OBJECT_JSONARRAYOBJECT_H_

#include "json_object/AbstractJsonContainer.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AbstractJsonObject;

class JsonArrayObject : public AbstractJsonContainer {
public:
	JsonArrayObject(const JsonArrayObject& inst);
	JsonArrayObject();
	virtual ~JsonArrayObject();

	virtual uint8_t getType() const noexcept {
		return JSON_TYPE_ARRAY_OBJECT;
	}

	void add(AbstractJsonObject* jsonObject) noexcept;
	int size() const noexcept;
	AbstractJsonObject* get(int pos) const noexcept;

	virtual AbstractJsonObject* copy() const noexcept;
	virtual bool equals(const AbstractJsonObject* other) const noexcept;

private:
	ArrayList<AbstractJsonObject>* list;
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_JSONARRAYOBJECT_H_ */
