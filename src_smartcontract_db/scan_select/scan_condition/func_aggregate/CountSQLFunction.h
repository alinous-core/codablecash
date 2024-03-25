/*
 * CountSQLFunction.h
 *
 *  Created on: 2021/10/31
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTION_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTION_H_

#include "scan_select/scan_condition/func/AbstractSQLAggregateFunctionBody.h"

namespace codablecash {

class CountSQLFunction: public AbstractSQLAggregateFunctionBody {
public:
	CountSQLFunction();
	virtual ~CountSQLFunction();

	virtual void analyze(AnalyzeContext* actx, SQLFunctionCall* fcall);

protected:
	virtual AbstractCdbValue* doGroupEvaluate(VirtualMachine* vm, const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata);

	AbstractCdbValue* countNotNull(VirtualMachine* vm, const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata);
	AbstractCdbValue* countAll(VirtualMachine* vm, const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata);
	AbstractCdbValue* countDistinct(VirtualMachine* vm, const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata);
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTION_H_ */
