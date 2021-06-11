/*
 * GroupByPlanner.cpp
 *
 *  Created on: 2021/05/10
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/GroupByPlanner.h"
#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_columns/ScanColumnHolder.h"
#include "scan_select/scan_columns/AbstractScanColumnsTarget.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "scan_select/scan_planner/analyze/AnalyzedScanPlan.h"

#include "scan_select/scan_planner/scanner/factory/AbstractScannerFactory.h"

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
	AnalyzedScanPlan* plan = planner->getPlan();
	AbstractScannerFactory* scannerFactory = plan->getScanFactory();
	const ScanResultMetadata* metadata = scannerFactory->getMetadata();

	this->columnHolder->resolveColumns(vm, planner);
}

CdbRecord* GroupByPlanner::groupBy(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	CdbRecord* groupedRecord = new CdbRecord();

	const ArrayList<AbstractScanColumnsTarget>* list = this->columnHolder->getList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* colTarget = list->get(i);

		colTarget->scanColumns(vm, record, metadata, groupedRecord);
	}

	return groupedRecord;
}

} /* namespace codablecash */
