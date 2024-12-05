/*
 * AbstractLockonOperation.cpp
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/AbstractLockinOperation.h"
#include "bc_status_cache_lockin/VotedLockinOperation.h"
#include "bc_status_cache_lockin/MissvoteLockinOperation.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

AbstractLockinOperation::AbstractLockinOperation(const AbstractLockinOperation &inst) {
	this->type = inst.type;
}

AbstractLockinOperation::AbstractLockinOperation(uint16_t type) {
	this->type = type;
}

AbstractLockinOperation::~AbstractLockinOperation() {

}

AbstractLockinOperation* AbstractLockinOperation::createFromBinary(ByteBuffer *in) {
	AbstractLockinOperation* op = nullptr;

	uint16_t type = in->getShort();

	switch(type){
	case TYPE_VOTED:
		op = new VotedLockinOperation();
		break;
	case TYPE_MISS_VOTED:
		op = new MissvoteLockinOperation();
		break;
	default:
		return nullptr;
	}

	op->fromBinary(in);

	return op;
}

bool AbstractLockinOperation::equals(const AbstractLockinOperation* op) const noexcept {
	return this->type == op->type && dataEquals(op);
}

} /* namespace codablecash */
