/*
 * AbstractSQLAggregateFunctionBody.cpp
 *
 *  Created on: 2021/10/26
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func/AbstractSQLAggregateFunctionBody.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "schema_table/record/table_record_value/CdbValueCastException.h"

#include "trx/scan/transaction_scan_result/GroupedScanResultMetadata.h"

namespace codablecash {

AbstractSQLAggregateFunctionBody::AbstractSQLAggregateFunctionBody(UnicodeString* name) : AbstractSQLFunctionBody(name) {

}

AbstractSQLAggregateFunctionBody::~AbstractSQLAggregateFunctionBody() {

}

bool AbstractSQLAggregateFunctionBody::isAggregate() const noexcept {
	return true;
}

AbstractCdbValue* AbstractSQLAggregateFunctionBody::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ArrayList<IValueProvider>* list, const ScanResultMetadata* metadata) {
	if(!record->isGrouped()){
		throw new CdbValueCastException(L"Aggregate function can be used for grouped record.", __FILE__, __LINE__);
	}

	const CdbGroupedRecord* grecord = dynamic_cast<const CdbGroupedRecord*>(record);
	const GroupedScanResultMetadata* gmetadata = dynamic_cast<const GroupedScanResultMetadata*>(metadata);

	return doGroupEvaluate(vm, grecord, list, gmetadata);
}

} /* namespace codablecash */
