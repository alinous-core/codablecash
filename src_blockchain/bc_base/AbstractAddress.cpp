/*
 * AbstractAddress.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_base/AbstractAddress.h"
#include "bc_base/Base58.h"
#include "bc_base/AddressDescriptor.h"
#include "bc_base/BalanceAddress.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

#include "osenv/funcs.h"



namespace codablecash {

AbstractAddress::AbstractAddress(uint16_t zone) {
	this->zone = zone;
}

AbstractAddress::~AbstractAddress() {

}

AbstractAddress* AbstractAddress::createFromBinary(ByteBuffer *in) {
	AbstractAddress* ret = nullptr;

	uint8_t type = in->get();
	switch(type){
	case AbstractAddress::ADDRESS_TYPE_BALANCE:
		ret = new BalanceAddress();
		break;
	default:
		return nullptr;
	}

	ret->fromBinary(in);

	return ret;
}

AddressDescriptor* AbstractAddress::toAddressDescriptor() const noexcept {
	const char* prefix = getPrefix();

	char zonech[4];
	Mem::memset(zonech, 0, 4);
	::sprintf(zonech, "%03d", this->zone);

	ByteBuffer* body = getBodyPart(); __STP(body);
	body->position(0);
	const char* charstr = (const char*)body->array();
	int length = body->limit();

	return new AddressDescriptor(prefix, zonech, charstr, length);
}

} /* namespace codablecash */
