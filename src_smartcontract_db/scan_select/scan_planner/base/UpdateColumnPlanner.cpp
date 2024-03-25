/*
 * UpdateColumnPlanner.cpp
 *
 *  Created on: 2022/01/08
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/UpdateColumnPlanner.h"

#include "scan_select/scan_planner/base/UpdateColumnPolicy.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "vm/VirtualMachine.h"

namespace codablecash {

UpdateColumnPlanner::UpdateColumnPlanner() {

}

UpdateColumnPlanner::~UpdateColumnPlanner() {
	this->list.deleteElements();
}

void UpdateColumnPlanner::addPolicy(UpdateColumnPolicy* p) noexcept {
	this->list.addElement(p);
}

CdbRecord* UpdateColumnPlanner::createUpdatedRecord(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	CdbRecord* newRecord = new CdbRecord(*record);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		UpdateColumnPolicy* policy = this->list.get(i);

		policy->updateRecord(vm, newRecord, metadata);
	}

	return newRecord;
}


} /* namespace codablecash */
