/*
 * IsNullScanColumnTarget.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_ISNULLSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_ISNULLSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

namespace codablecash {

class IsNullScanCondition;

class IsNullScanColumnTarget : public AbstractExpressionScanColumnTarget {
public:
	IsNullScanColumnTarget();
	virtual ~IsNullScanColumnTarget();

	void setCondition(AbstractScanColumnsTarget* cond) noexcept;
	void setIsNull(bool notnull) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	AbstractScanColumnsTarget* innerCond;
	bool notnull;

	IsNullScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_ISNULLSCANCOLUMNTARGET_H_ */
