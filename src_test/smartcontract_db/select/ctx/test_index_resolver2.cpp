/*
 * test_index_resolver2.cpp
 *
 *  Created on: 2022/01/15
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "../../toolkit/TestDbSchema01.h"
#include "vm/VirtualMachine.h"
#include "vm/VmSelectPlannerSetter.h"

#include "scan_select/scan_planner/scanner/index_resolv/IndexResolver.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"
#include "scan_select/scan_planner/base/TablesHolder.h"

#include "scan_select/scan_table/TableScanTarget.h"

#include "lang_sql/sql_expression/SQLColumnIdentifier.h"

#include "scan_select/scan_condition/exp/InExpressionScanCondition.h"
#include "scan_select/scan_condition/exp/ExpressionListScanCondition.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "scan_select/scan_condition/params/NumericScanParam.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"

#include "scan_select/scan_planner/scanner/factory/AbstractScannerFactory.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

#include "vm/vm_trx/VmTransactionHandler.h"

#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

#include "scan_select/scan_condition/exp/BetweenScanCondition.h"

#include "scan_select/scan_condition/exp/ParenthesisScanCondition.h"
using namespace codablecash;

TEST_GROUP(TestIndexResolver2Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestIndexResolver2Group, InExpressionScanCondition01){
	TestDbSchema01 tester(this->env);
	tester.init(1024 * 10);

	VirtualMachine* vm = tester.getVm();
	{
		IndexResolver resolver(tester.getDatabase());

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		TablesHolder* tableHolder = planner->getTablesHolder();
		TableScanTarget* target = tester.getScanTarget(L"public", L"test_table");
		tableHolder->addScanTarget(target);

		ConditionsHolder* cholder = planner->getConditions();
		RootScanCondition* root = cholder->getRoot();


			InExpressionScanCondition* inExp = new InExpressionScanCondition();

			SQLColumnIdentifier id(L"public", L"test_table", L"id");
			inExp->setLeft(new ColumnIdentifierScanParam(&id));

			ExpressionListScanCondition* list = new ExpressionListScanCondition();
			list->addElement(new NumericScanParam(1));
			inExp->setList(list);

			CHECK(inExp->isRecordIndependent() == false);

		// add to root
		root->addCondition(inExp);


		root->analyzeConditions(vm, planner);
		AbstractScannerFactory* f = tableHolder->buildScanFactory(vm, planner); __STP(f);

		//
		VmTransactionHandler* trxHandler = vm->getTransactionHandler();
		trxHandler->begin();

		IJoinLeftSource* leftSource = f->createScannerAsLeftSource(vm ,planner); __STP(leftSource);

		trxHandler->rollback();
	}
}

TEST(TestIndexResolver2Group, ExpressionListScanCondition01){
	TestDbSchema01 tester(this->env);
	tester.init(1024 * 10);

	VirtualMachine* vm = tester.getVm();
	{
		IndexResolver resolver(tester.getDatabase());

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		TablesHolder* tableHolder = planner->getTablesHolder();
		TableScanTarget* target = tester.getScanTarget(L"public", L"test_table");
		tableHolder->addScanTarget(target);

		ConditionsHolder* cholder = planner->getConditions();
		RootScanCondition* root = cholder->getRoot();

		/////
		SQLColumnIdentifier id(L"public", L"test_table", L"id");

		ExpressionListScanCondition* list = new ExpressionListScanCondition();
		list->addElement(new NumericScanParam(1));
		list->addElement(new ColumnIdentifierScanParam(&id));
		/////

		// add to root
		root->addCondition(list);

		root->analyzeConditions(vm, planner);

		TableIndexDetector indexDetector(vm, planner, target);
		root->detectIndexCandidate(vm ,planner, &indexDetector);

		bool bl = list->hasColumnId();
		CHECK(bl == true);

		FilterConditionDitector fdetector(vm, planner);
		list->detectFilterConditions(vm, planner, &fdetector);
		CHECK(fdetector.isEmpty());
	}
}

TEST(TestIndexResolver2Group, BetweenScanCondition01){
	TestDbSchema01 tester(this->env);
	tester.init(1024 * 10);

	VirtualMachine* vm = tester.getVm();
	{
		IndexResolver resolver(tester.getDatabase());

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		TablesHolder* tableHolder = planner->getTablesHolder();
		TableScanTarget* target = tester.getScanTarget(L"public", L"test_table");
		tableHolder->addScanTarget(target);

		ConditionsHolder* cholder = planner->getConditions();
		RootScanCondition* root = cholder->getRoot();

		/////
		SQLColumnIdentifier id(L"public", L"test_table", L"id");

		BetweenScanCondition* cond = new BetweenScanCondition();
		cond->setLeft(new ColumnIdentifierScanParam(&id));
		cond->setStart(new NumericScanParam(1));
		cond->setEnd(new NumericScanParam(10));
		/////

		CHECK(!cond->isRecordIndependent());

		root->addCondition(cond);

		root->analyzeConditions(vm, planner);
		AbstractScannerFactory* f = tableHolder->buildScanFactory(vm, planner); __STP(f);

		//
		VmTransactionHandler* trxHandler = vm->getTransactionHandler();
		trxHandler->begin();

		IJoinLeftSource* leftSource = f->createScannerAsLeftSource(vm ,planner); __STP(leftSource);

		trxHandler->rollback();
	}
}

TEST(TestIndexResolver2Group, ParenthesisScanCondition01){
	TestDbSchema01 tester(this->env);
	tester.init(1024 * 10);

	VirtualMachine* vm = tester.getVm();
	{
		IndexResolver resolver(tester.getDatabase());

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		TablesHolder* tableHolder = planner->getTablesHolder();
		TableScanTarget* target = tester.getScanTarget(L"public", L"test_table");
		tableHolder->addScanTarget(target);

		ConditionsHolder* cholder = planner->getConditions();
		RootScanCondition* root = cholder->getRoot();

		/////
		SQLColumnIdentifier id(L"public", L"test_table", L"id");

		BetweenScanCondition* cond = new BetweenScanCondition();
		cond->setLeft(new ColumnIdentifierScanParam(&id));
		cond->setStart(new NumericScanParam(1));
		cond->setEnd(new NumericScanParam(10));

		CHECK(!cond->isRecordIndependent());

		ParenthesisScanCondition* pcond = new ParenthesisScanCondition();
		pcond->addCondition(cond);
		/////

		root->addCondition(pcond);

		root->analyzeConditions(vm, planner);
		AbstractScannerFactory* f = tableHolder->buildScanFactory(vm, planner); __STP(f);

		//
		VmTransactionHandler* trxHandler = vm->getTransactionHandler();
		trxHandler->begin();

		IJoinLeftSource* leftSource = f->createScannerAsLeftSource(vm ,planner); __STP(leftSource);

		trxHandler->rollback();

	}
}
