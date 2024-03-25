/*
 * CdbTableIndexCompare.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_TABLE_CDBTABLEINDEXCOMPARE_H_
#define SCHEMA_TABLE_TABLE_CDBTABLEINDEXCOMPARE_H_

namespace codablecash {

class CdbTableIndex;

class CdbTableIndexCompare {
public:
	int operator() (const CdbTableIndex* const a, const CdbTableIndex* const b) const noexcept;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_TABLE_CDBTABLEINDEXCOMPARE_H_ */
