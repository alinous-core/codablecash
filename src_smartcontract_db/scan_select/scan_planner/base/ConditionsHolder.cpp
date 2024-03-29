/*
 * ConditionsHolder.cpp
 *
 *  Created on: 2020/07/19
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/ConditionsHolder.h"
#include "scan_select/scan_planner/base/ConditionsHolderStack.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "lang_sql/sql/AbstractSQLExpression.h"

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"


namespace codablecash {

ConditionsHolder::ConditionsHolder() {
	this->root = new RootScanCondition();
	this->havingRoot = new RootScanCondition();
	this->stack = new ConditionsHolderStack();
}

ConditionsHolder::~ConditionsHolder() {
	delete this->stack;
	delete this->root;
	delete this->havingRoot;
}

void ConditionsHolder::push(AbstractScanConditionElement* cond) noexcept {
	this->stack->push(cond);
}

AbstractScanConditionElement* ConditionsHolder::top() const noexcept {
	int index = this->stack->size() - 1;
	return this->stack->get(index);
}

AbstractScanConditionElement* ConditionsHolder::pop() noexcept {
	return this->stack->pop();
}

RootScanCondition* ConditionsHolder::getRoot() const noexcept {
	return this->root;
}

RootScanCondition* ConditionsHolder::getHavingRoot() const noexcept {
	return this->havingRoot;
}

bool ConditionsHolder::hasHavingCondition() const noexcept {
	return this->havingRoot->hasCondition();
}

void ConditionsHolder::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->root->analyzeConditions(vm, planner);
}

void ConditionsHolder::analyzeHavingConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->havingRoot->analyzeConditions(vm, planner);
}

} /* namespace codablecash */
