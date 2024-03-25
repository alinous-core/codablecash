/*
 * AbstractConfigStoreElement.cpp
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#include "bc_base_conf_store/AbstractConfigStoreElement.h"
#include "bc_base_conf_store/LongValueConfigStoreValue.h"
#include "bc_base_conf_store/ShortValueConfigStoreValue.h"
#include "bc_base_conf_store/BinaryValueConfigStoreValue.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

AbstractConfigStoreElement::AbstractConfigStoreElement(uint8_t type) {
	this->type = type;
}

AbstractConfigStoreElement::~AbstractConfigStoreElement() {

}

AbstractConfigStoreElement* AbstractConfigStoreElement::createFromBinary(ByteBuffer* in) {
	uint8_t t = in->get();
	AbstractConfigStoreElement* ret = nullptr;

	if(t == TYPE_LONG_VALUE){
		ret = new LongValueConfigStoreValue();
	}
	else if(t == TYPE_SHORT_VALUE){
		ret = new ShortValueConfigStoreValue();
	}
	else if(t == TYPE_BINARY_VALUE){
		ret = new BinaryValueConfigStoreValue();
	}
	else{
		return nullptr;
	}

	ret->fromBinary(in);

	return ret;
}

} /* namespace codablecash */
