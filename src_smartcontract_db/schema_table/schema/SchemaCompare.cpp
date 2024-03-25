/*
 * SchemaCompare.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "schema_table/schema/SchemaCompare.h"
#include "schema_table/schema/Schema.h"

#include "engine/CdbOid.h"

namespace codablecash {

int SchemaCompare::operator ()(const Schema *const a, const Schema *const b) const noexcept {
	CdbOid::ValueCompare cmp;

	const CdbOid* oida = a->getOid();
	const CdbOid* oidb = b->getOid();

	return cmp(oida, oidb);
}

} /* namespace codablecash */
