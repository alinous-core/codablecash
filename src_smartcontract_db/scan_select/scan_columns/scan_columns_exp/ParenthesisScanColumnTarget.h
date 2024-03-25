/*
 * ParenthesisScanColumnTarget.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_PARENTHESISSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_PARENTHESISSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

namespace codablecash {

class ParenthesisScanCondition;

class ParenthesisScanColumnTarget: public AbstractExpressionScanColumnTarget {
public:
	ParenthesisScanColumnTarget();
	virtual ~ParenthesisScanColumnTarget();

	void setInnser(AbstractScanColumnsTarget* cond) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	AbstractScanColumnsTarget* innerCond;

	ParenthesisScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_PARENTHESISSCANCOLUMNTARGET_H_ */
