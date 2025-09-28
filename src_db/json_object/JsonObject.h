/*
 * JsonObject.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_OBJECT_JSONOBJECT_H_
#define JSON_OBJECT_JSONOBJECT_H_

#include "json_object/AbstractJsonContainer.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class JsonValuePair;

class JsonObject : public AbstractJsonContainer {
public:
	JsonObject();
	virtual ~JsonObject();

	virtual uint8_t getType() const noexcept {
		return JSON_TYPE_OBJECT;
	}

	void add(JsonValuePair* pair) noexcept;

	const JsonValuePair* get(int pos) const noexcept;

private:
	ArrayList<JsonValuePair>* list;
};

} /* namespace codablecash */

#endif /* JSON_OBJECT_JSONOBJECT_H_ */
