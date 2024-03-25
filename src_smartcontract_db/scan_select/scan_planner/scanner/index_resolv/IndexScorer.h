/*
 * IndexScorer.h
 *
 *  Created on: 2023/06/28
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_INDEXSCORER_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_INDEXSCORER_H_

#include "base/ArrayList.h"

#include <cstdint>

using namespace alinous;

namespace codablecash {

class SingleColumnIndexAccess;
class CdbTableIndex;
class CdbTableColumn;
class CdbTable;

class IndexScorer {
public:
	IndexScorer();
	virtual ~IndexScorer();

	void add(const SingleColumnIndexAccess* access) noexcept;

	uint64_t socre(CdbTableIndex* index, const CdbTable* table);

private:
	const SingleColumnIndexAccess* findAccess(CdbTableColumn* col);

private:
	ArrayList<const SingleColumnIndexAccess> list;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_INDEXSCORER_H_ */
