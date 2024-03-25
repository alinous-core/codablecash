/*
 * ExpressionListScanColumnTarget.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_EXPRESSIONLISTSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_EXPRESSIONLISTSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

#include "base/ArrayList.h"

namespace codablecash {

class ExpressionListScanCondition;

class ExpressionListScanColumnTarget: public AbstractExpressionScanColumnTarget {
public:
	ExpressionListScanColumnTarget();
	virtual ~ExpressionListScanColumnTarget();

	void addElement(AbstractScanColumnsTarget* val) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	ArrayList<AbstractScanColumnsTarget> list;

	ExpressionListScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_EXPRESSIONLISTSCANCOLUMNTARGET_H_ */
