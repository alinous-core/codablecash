/*
 * CdbTableCompare.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_TABLE_CDBTABLECOMPARE_H_
#define SCHEMA_TABLE_TABLE_CDBTABLECOMPARE_H_

namespace codablecash {

class CdbTable;

class CdbTableCompare {
public:
	int operator() (const CdbTable* const a, const CdbTable* const b) const noexcept;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_TABLE_CDBTABLECOMPARE_H_ */
