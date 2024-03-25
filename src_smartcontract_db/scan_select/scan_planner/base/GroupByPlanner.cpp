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

#include "trx/scan/transaction_scan_result/GroupedScanResultMetadata.h"

#include "scan_select/scan_columns/ScanColumn.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "engine/CdbException.h"
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

GroupedScanResultMetadata* GroupByPlanner::getMetadata(const ScanResultMetadata* base) {
	GroupedScanResultMetadata* meta = new GroupedScanResultMetadata(base);

	const ArrayList<AbstractScanColumnsTarget>* list = this->columnHolder->getList();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* colTarget = list->get(i);

		ScanColumn* scanColumn = dynamic_cast<ScanColumn*>(colTarget);
		const ColumnIdentifierScanParam* colParam = scanColumn->getScanParam();

		const ScanResultFieldMetadata* fldMetadata = base->findField(colParam);
		if(fldMetadata == nullptr){
			throw new CdbException(L"Can not find field from metadata", __FILE__, __LINE__);
		}

		meta->addField(fldMetadata);
	}

	return meta;
}

bool GroupByPlanner::isGroupBy() const noexcept {
	const ArrayList<AbstractScanColumnsTarget>* list = this->columnHolder->getList();

	return !list->isEmpty();
}

} /* namespace codablecash */
