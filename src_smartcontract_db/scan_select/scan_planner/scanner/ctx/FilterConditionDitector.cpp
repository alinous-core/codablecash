/*
 * FilterConditionDitector.cpp
 *
 *  Created on: 2020/09/02
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStack.h"

#include "scan_select/scan_table/AbstractScanTableTarget.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"
#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_table/AbstractJoinScanTarget.h"

#include "scan_select/scan_condition/logical/AndScanCondition.h"
namespace codablecash {

FilterConditionDitector::FilterConditionDitector(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->vm = vm;
	this->planner = planner;
	this->cond = nullptr;
	this->stack = new FilterConditionStack();
}

FilterConditionDitector::~FilterConditionDitector() {
	delete this->stack;

	this->vm = nullptr;
	this->planner = nullptr;

	delete this->cond;
}

void FilterConditionDitector::detect(AbstractScanTableTarget* target) {
	ConditionsHolder* holder = planner->getConditions();

	target->collectScanTargets(this->vm, this->planner, &this->list);

	RootScanCondition* root = holder->getRoot();
	root->detectFilterConditions(this->vm, this->planner, this);
	if(!isEmpty()){
		this->cond = pop();
	}

	// ON clause
	AbstractJoinScanTarget* joinTarget = dynamic_cast<AbstractJoinScanTarget*>(target);
	if(joinTarget != nullptr){
		AbstractScanCondition* onClause = joinTarget->getCondition();

		if(onClause != nullptr){
			onClause->detectFilterConditions(this->vm, this->planner, this);
			if(!isEmpty()){
				AbstractScanCondition* con = pop();

				AndScanCondition* andCond = new AndScanCondition();
				andCond->addCondition(con);
				andCond->addCondition(this->cond);

				this->cond = andCond;
			}
		}
	}

}

bool FilterConditionDitector::hasTarget(const AbstractScanTableTarget* target) const noexcept {
	bool result = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanTableTarget* t = this->list.get(i);

		if(t == target){
			result = true;
			break;
		}
	}

	return result;
}

bool FilterConditionDitector::isEmpty() const noexcept {
	return this->stack->isEmpty();
}

void FilterConditionDitector::push(AbstractScanCondition* cond) noexcept {
	this->stack->push(cond);
}

AbstractScanCondition* FilterConditionDitector::pop() noexcept {
	return this->stack->pop();
}

} /* namespace codablecash */
