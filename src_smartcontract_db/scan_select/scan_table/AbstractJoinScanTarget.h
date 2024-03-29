/*
 * AbstractJoinScanTarget.h
 *
 *  Created on: 2020/08/09
 *      Author: iizuka
 */

#ifndef SCAN_TABLE_ABSTRACTJOINSCANTARGET_H_
#define SCAN_TABLE_ABSTRACTJOINSCANTARGET_H_

#include "scan_select/scan_table/AbstractScanTableTarget.h"

namespace alinous{
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractScanCondition;

class AbstractJoinScanTarget : public AbstractScanTableTarget {
public:
	AbstractJoinScanTarget();
	virtual ~AbstractJoinScanTarget();

	virtual void setLeft(AbstractScanTableTarget* left) noexcept;
	virtual void setRight(AbstractScanTableTarget* right) noexcept;

	void setCondition(AbstractScanCondition* cond) noexcept;
	AbstractScanCondition* getCondition() const noexcept {
		return this->cond;
	}

	virtual void collectScanTargets(VirtualMachine* vm, SelectScanPlanner* planner, ArrayList<AbstractScanTableTarget>* list);
	virtual void resolveTable(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual bool hasTarget(const AbstractScanTableTarget* target) const noexcept;

	virtual ScanTableColumnParam* findTableColumns(const UnicodeString* colName) const;

protected:
	void resetStr() noexcept;
	void addConditionString() noexcept;

protected:
	AbstractScanTableTarget* left;
	AbstractScanTableTarget* right;

	AbstractScanCondition* cond; // ON

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_TABLE_ABSTRACTJOINSCANTARGET_H_ */
