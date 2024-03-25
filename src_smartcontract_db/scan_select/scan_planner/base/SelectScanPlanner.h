/*
 * SelectScanPlanner.h
 *
 *  Created on: 2020/07/19
 *      Author: iizuka
 */

#ifndef SCAN_PLANNER_SELECTSCANPLANNER_H_
#define SCAN_PLANNER_SELECTSCANPLANNER_H_

namespace alinous {
class AbstractSQLExpression;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class ConditionsHolder;
class AbstractScanCondition;
class AbstractScanConditionElement;
class TablesHolder;
class ScanColumnHolder;
class AnalyzedScanPlan;
class ConditionsHolderStack;
class GroupByPlanner;
class ScanResultExecutor;

class SelectScanPlanner {
public:
	SelectScanPlanner();
	virtual ~SelectScanPlanner();

	void push(AbstractScanConditionElement* cond) noexcept;
	AbstractScanConditionElement* top() const noexcept;
	AbstractScanConditionElement* pop() noexcept;

	ConditionsHolder* getConditions() const noexcept {
		return this->conditions;
	}
	ConditionsHolderStack* getConditionsStack() const noexcept;

	TablesHolder* getTablesHolder() const noexcept {
		return tablesHolder;
	}

	ScanColumnHolder* getColumnHolder() const noexcept {
		return columnHolder;
	}

	void makeplan(VirtualMachine* vm);
	void resolveColumn(VirtualMachine* vm);

	void executeQuery(VirtualMachine* vm);
	ScanResultExecutor* executeQueryScan(VirtualMachine* vm);

	AnalyzedScanPlan* getPlan() const noexcept {
		return plan;
	}

	GroupByPlanner* getGroupPlan() noexcept;

private:
	void resolveTable(VirtualMachine* vm);

	void analyzeConditions(VirtualMachine* vm);

	void buildScannerFactories(VirtualMachine* vm);

private:
	ScanColumnHolder* columnHolder;
	ConditionsHolder* conditions;

	TablesHolder* tablesHolder;

	AnalyzedScanPlan* plan;
	GroupByPlanner* groupPlan;
};

} /* namespace codablecash */

#endif /* SCAN_PLANNER_SELECTSCANPLANNER_H_ */
