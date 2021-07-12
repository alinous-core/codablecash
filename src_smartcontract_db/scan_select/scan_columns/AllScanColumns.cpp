/*
 * AllScanColumns.cpp
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#include "scan_select/scan_columns/AllScanColumns.h"

#include "base/UnicodeString.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

AllScanColumns::AllScanColumns() {

}

AllScanColumns::~AllScanColumns() {

}

const UnicodeString* AllScanColumns::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"*");
	}

	return this->str;
}

void AllScanColumns::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {

}

void AllScanColumns::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	const ArrayList<AbstractCdbValue>* list = record->getValues();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractCdbValue* v = list->get(i);

		newRecord->addValue(v != nullptr ? v->copy() : nullptr);
	}
}

} /* namespace codablecash */
