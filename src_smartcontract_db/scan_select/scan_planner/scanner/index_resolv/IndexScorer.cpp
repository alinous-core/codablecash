/*
 * IndexScorer.cpp
 *
 *  Created on: 2023/06/28
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/index_resolv/IndexScorer.h"
#include "scan_select/scan_planner/scanner/index_resolv/SingleColumnIndexAccess.h"

#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/table/CdbTableColumn.h"

#include "engine/CdbOid.h"

#include "schema_table/table/CdbTable.h"
namespace codablecash {

IndexScorer::IndexScorer() {

}

IndexScorer::~IndexScorer() {

}

void IndexScorer::add(const SingleColumnIndexAccess *access) noexcept {
	this->list.addElement(access);
}

uint64_t IndexScorer::socre(CdbTableIndex *index, const CdbTable* table) {
	uint64_t score = 0L;

	const ArrayList<CdbOid>* cols = index->getColumns();
	int maxLoop = cols->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* colOid = cols->get(i);
		CdbTableColumn* col = table->getColumn(colOid);

		const SingleColumnIndexAccess* access = findAccess(col);
		if(access == nullptr){
			break;
		}

		if(access->hasPlaceHolder()){
			score += 1;
		}
		else{
			score += 100;
		}
	}

	return score;
}

const SingleColumnIndexAccess* IndexScorer::findAccess(CdbTableColumn *col) {
	const SingleColumnIndexAccess* ret = nullptr;

	const CdbOid* oid = col->getOid();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		const SingleColumnIndexAccess* access = this->list.get(i);
		const CdbTableColumn* c = access->getColumn();
		const CdbOid* colId = c->getOid();

		if(oid->equals(colId)){
			ret = access;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
