/*
 * OidWraper.cpp
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#include "engine/OidWraper.h"
#include "engine/CdbOid.h"

namespace codablecash {

OidWraper::OidWraper(const OidWraper &inst) {
	this->oid = inst.oid != nullptr ? inst.oid->copy() : nullptr;
}

OidWraper::OidWraper(const CdbOid* oid) {
	this->oid = oid->copy();
}

OidWraper::~OidWraper() {
	delete this->oid;
}

int OidWraper::ValueCompare::operator()(const OidWraper *const _this, const OidWraper *const object) const noexcept {
	static CdbOid::ValueCompare comp;

	return comp(_this->oid, object->oid);
}

int OidWraper::hashCode() const {
	return this->oid->hashCode();
}

} /* namespace codablecash */

