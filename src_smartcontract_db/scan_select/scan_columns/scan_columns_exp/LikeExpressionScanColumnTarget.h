/*
 * LikeExpression.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_LIKEEXPRESSIONSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_LIKEEXPRESSIONSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

namespace codablecash {

class LikeScanCondition;

class LikeExpressionScanColumnTarget: public AbstractExpressionScanColumnTarget {
public:
	LikeExpressionScanColumnTarget();
	virtual ~LikeExpressionScanColumnTarget();

	void setLeft(AbstractScanColumnsTarget* left) noexcept;
	void setRight(AbstractScanColumnsTarget* right) noexcept;
	void setEscape(AbstractScanColumnsTarget* escape) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	AbstractScanColumnsTarget* left;
	AbstractScanColumnsTarget* right;
	AbstractScanColumnsTarget* escape;

	LikeScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_LIKEEXPRESSIONSCANCOLUMNTARGET_H_ */
