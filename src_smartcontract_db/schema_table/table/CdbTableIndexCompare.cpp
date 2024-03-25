/*
 * CdbTableIndexCompare.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "schema_table/table/CdbTableIndexCompare.h"
#include "schema_table/table/CdbTableIndex.h"

#include "engine/CdbOid.h"

namespace codablecash {

int CdbTableIndexCompare::operator ()(const CdbTableIndex *const a, const CdbTableIndex *const b) const noexcept {
	static CdbOid::ValueCompare cmp;

	const CdbOid* oida = a->getOid();
	const CdbOid* oidb = b->getOid();

	return cmp(oida, oidb);
}

} /* namespace codablecash */
