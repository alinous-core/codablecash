/*
 * CdbTableCompare.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "schema_table/table/CdbTableCompare.h"
#include "schema_table/table/CdbTable.h"

#include "engine/CdbOid.h"

namespace codablecash {

int CdbTableCompare::operator ()(const CdbTable *const a, const CdbTable *const b) const noexcept {
	CdbOid::ValueCompare cmp;

	const CdbOid* oida = a->getOid();
	const CdbOid* oidb = b->getOid();

	return cmp(oida, oidb);
}

} /* namespace codablecash */
