/*
 * BetweenScanColumnTarget.h
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_BETWEENSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_BETWEENSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

namespace codablecash {

class BetweenScanCondition;

class BetweenScanColumnTarget : public AbstractExpressionScanColumnTarget {
public:
	BetweenScanColumnTarget();
	virtual ~BetweenScanColumnTarget();

	void setLeft(AbstractScanColumnsTarget* left) noexcept;
	void setStart(AbstractScanColumnsTarget* start) noexcept;
	void setEnd(AbstractScanColumnsTarget* end) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();
private:
	AbstractScanColumnsTarget* left;
	AbstractScanColumnsTarget* start;
	AbstractScanColumnsTarget* end;

	BetweenScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_BETWEENSCANCOLUMNTARGET_H_ */
