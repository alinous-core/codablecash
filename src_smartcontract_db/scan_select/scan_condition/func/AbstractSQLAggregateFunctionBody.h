/*
 * AbstractSQLAggregateFunctionBody.h
 *
 *  Created on: 2021/10/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLAGGREGATEFUNCTIONBODY_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLAGGREGATEFUNCTIONBODY_H_

#include "scan_select/scan_condition/func/AbstractSQLFunctionBody.h"

namespace codablecash {

class CdbGroupedRecord;
class GroupedScanResultMetadata;

class AbstractSQLAggregateFunctionBody : public AbstractSQLFunctionBody {
public:
	explicit AbstractSQLAggregateFunctionBody(UnicodeString* name);
	virtual ~AbstractSQLAggregateFunctionBody();

	virtual bool isAggregate() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ArrayList<IValueProvider>* list, const ScanResultMetadata* metadata);

protected:
	virtual AbstractCdbValue* doGroupEvaluate(VirtualMachine* vm, const CdbGroupedRecord* record, const ArrayList<IValueProvider>* list, const GroupedScanResultMetadata* metadata) = 0;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLAGGREGATEFUNCTIONBODY_H_ */
