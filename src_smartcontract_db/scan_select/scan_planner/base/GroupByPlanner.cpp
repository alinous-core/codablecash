/*
 * GroupByPlanner.cpp
 *
 *  Created on: 2021/05/10
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/GroupByPlanner.h"

#include "scan_select/scan_columns/ScanColumnHolder.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

GroupByPlanner::GroupByPlanner() {
	this->columnHolder = new ScanColumnHolder();
}

GroupByPlanner::~GroupByPlanner() {
	delete this->columnHolder;
}

void codablecash::GroupByPlanner::addColumn(AbstractScanColumnsTarget* col) {
	this->columnHolder->addColumn(col);
}

void GroupByPlanner::resolveColumns(VirtualMachine* vm,	SelectScanPlanner* planner) {
	this->columnHolder->resolveColumns(vm, planner);
}

CdbRecord* GroupByPlanner::groupBy(const CdbRecord* record, const ScanResultMetadata* metadata) {
	CdbRecord* groupedRecord = new CdbRecord();

	const ArrayList<AbstractScanColumnsTarget>* list = this->columnHolder->getList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* colTarget = list->get(i);


	}

	// TODO:
	return groupedRecord;
}

} /* namespace codablecash */
