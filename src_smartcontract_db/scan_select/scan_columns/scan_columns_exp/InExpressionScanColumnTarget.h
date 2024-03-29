/*
 * InExpressionScanColumnTarget.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_EXP_INEXPRESSIONSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_EXP_INEXPRESSIONSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_exp/AbstractExpressionScanColumnTarget.h"

namespace alinous {
class CodeElement;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class ExpressionListScanColumnTarget;
class InExpressionScanCondition;

class InExpressionScanColumnTarget: public AbstractExpressionScanColumnTarget {
public:
	InExpressionScanColumnTarget();
	virtual ~InExpressionScanColumnTarget();

	void setLeft(AbstractScanColumnsTarget* cond) noexcept;
	void setList(ExpressionListScanColumnTarget* list) noexcept;

	static ExpressionListScanColumnTarget* castToExpressionListScanColumnTarget(AbstractScanColumnsTarget* col, VirtualMachine* vm, const CodeElement* element);

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	AbstractScanColumnsTarget* left;
	ExpressionListScanColumnTarget* list;

	InExpressionScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_EXP_INEXPRESSIONSCANCOLUMNTARGET_H_ */
