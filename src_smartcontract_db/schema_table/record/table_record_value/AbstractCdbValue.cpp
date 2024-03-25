/*
 * AbstractCdbValue.cpp
 *
 *  Created on: 2020/05/20
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

namespace codablecash {

AbstractCdbValue::AbstractCdbValue(uint8_t type) {
	this->type = type;
}

AbstractCdbValue::~AbstractCdbValue() {

}

IBlockObject* AbstractCdbValue::copyData() const noexcept {
	return copy();
}

bool AbstractCdbValue::strictEquals(const AbstractCdbValue *v) const {
	if(v == nullptr || v->type != this->type){
		return false;
	}

	return strictEqualsValue(v);
}

} /* namespace codablecash */
