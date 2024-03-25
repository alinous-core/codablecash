/*
 * SelectScanPlanner.cpp
 *
 *  Created on: 2020/07/19
 *      Author: iizuka
 */

#include <cassert>

#include "scan_select/scan_planner/base/SelectScanPlanner.h"
#include "scan_select/scan_planner/base/ConditionsHolder.h"
#include "scan_select/scan_planner/base/TablesHolder.h"
#include "scan_select/scan_planner/base/ConditionsHolderStack.h"
#include "scan_select/scan_planner/base/GroupByPlanner.h"

#include "scan_select/scan_planner/analyze/AnalyzedScanPlan.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "scan_select/scan_columns/ScanColumnHolder.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_planner/scanner/factory/AbstractScannerFactory.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "trx/scan/transaction_scan_result/ScanResultExecutor.h"

#include "base/StackRelease.h"

#include "vm/VmSelectPlannerSetter.h"


namespace codablecash {

SelectScanPlanner::SelectScanPlanner() {
	this->conditions = new ConditionsHolder();
	this->tablesHolder = new TablesHolder();
	this->columnHolder = new ScanColumnHolder();
	this->plan = new AnalyzedScanPlan();
	this->groupPlan = nullptr;
}

SelectScanPlanner::~SelectScanPlanner() {
	delete this->conditions;
	delete this->tablesHolder;
	delete this->columnHolder;
	delete this->plan;
	delete this->groupPlan;
}

void SelectScanPlanner::push(AbstractScanConditionElement* cond) noexcept {
	this->conditions->push(cond);
}

AbstractScanConditionElement* SelectScanPlanner::top() const noexcept {
	return this->conditions->top();
}

AbstractScanConditionElement* SelectScanPlanner::pop() noexcept{
	return this->conditions->pop();
}


void SelectScanPlanner::makeplan(VirtualMachine* vm) {
	delete this->plan;
	this->plan = new AnalyzedScanPlan();

	resolveTable(vm);

	analyzeConditions(vm);

	buildScannerFactories(vm);

	AbstractScannerFactory* factory = this->plan->getScanFactory();
	factory->analyzeConditions(vm, this);
}

void SelectScanPlanner::resolveTable(VirtualMachine* vm) {
	this->tablesHolder->resolveTable(vm, this);
}

void SelectScanPlanner::resolveColumn(VirtualMachine* vm) {
	if(this->groupPlan != nullptr){
		this->groupPlan->resolveColumns(vm, this);
	}

	this->columnHolder->resolveColumns(vm, this);
}

void SelectScanPlanner::analyzeConditions(VirtualMachine* vm) {
	this->conditions->analyzeConditions(vm, this);
}

GroupByPlanner* SelectScanPlanner::getGroupPlan() noexcept {
	if(this->groupPlan == nullptr){
		this->groupPlan = new GroupByPlanner();
	}

	return this->groupPlan;
}

void SelectScanPlanner::buildScannerFactories(VirtualMachine* vm) {
	this->tablesHolder->buildScannerFactories(vm, this);
}

void SelectScanPlanner::executeQuery(VirtualMachine* vm) {
	CodableDatabase* db = vm->getDb();

	AbstractScannerFactory* scanFactory = this->plan->getScanFactory();

	IJoinLeftSource* left = scanFactory->createScannerAsLeftSource(vm, this);

	VmSelectPlannerSetter setter(vm, this);

	ScanResultExecutor exec(left, db);
	exec.init(vm);
	exec.execScan(vm);

	exec.putResult(vm, this);
}

ScanResultExecutor* SelectScanPlanner::executeQueryScan(VirtualMachine* vm) {
	CodableDatabase* db = vm->getDb();

	AbstractScannerFactory* scanFactory = this->plan->getScanFactory();

	IJoinLeftSource* left = scanFactory->createScannerAsLeftSource(vm, this);

	VmSelectPlannerSetter setter(vm, this);

	ScanResultExecutor* exec = new ScanResultExecutor(left, db);
	exec->init(vm);
	exec->execScan(vm);

	return exec;
}

ConditionsHolderStack* SelectScanPlanner::getConditionsStack() const noexcept {
	return this->conditions->getStack();
}


} /* namespace codablecash */
