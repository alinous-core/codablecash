/*
 * test_select_arithmetic_contition2.cpp
 *
 *  Created on: 2023/06/05
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit/TestDbSchema02.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "engine/sc_analyze_stack/AnalyzeStackPopper.h"

#include "engine/sc_analyze_stack/AnalyzeStackManager.h"

#include "vm/VmSelectPlannerSetter.h"

#include "vm/variable_access/StackVariableAccess.h"

#include "instance/instance_gc/GcManager.h"

#include "vm/VirtualMachine.h"

#include "base/StackRelease.h"

#include "vm/stack/StackPopper.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_condition/exp/EqualityScanCondition.h"

#include "schema_table/record/table_record_value/CdbLongValue.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
using namespace codablecash;


TEST_GROUP(TestSelectArithmeticCondition2Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestSelectArithmeticCondition2Group, AdditiveScanCondition01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/arithmetic_select/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.execSelectStmt(stmt);

			VirtualMachine* vm = schem.getVm();

			SelectScanPlanner* planner = stmt->getSelectScanPlanner();
			ConditionsHolder* holder = planner->getConditions();
			RootScanCondition* root = holder->getRoot();

			AbstractScanCondition* cond = root->getCondition();

			EqualityScanCondition* eqcond = dynamic_cast<EqualityScanCondition*>(cond);
			AbstractScanCondition* right = dynamic_cast<AbstractScanCondition*>(eqcond->getRight());

			CHECK(right->hasColumnId() == true);

			FilterConditionDitector detector(vm, planner);
			right->detectFilterConditions(vm, planner, &detector);
		}
	}
}

TEST(TestSelectArithmeticCondition2Group, AdditiveScanCondition02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/arithmetic_select/select02.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.execSelectStmt(stmt);

			{
				SQLWhere* w = stmt->getWhereClause();
				AbstractSQLExpression* exp = w->getExp();
			}
		}
	}
}

TEST(TestSelectArithmeticCondition2Group, AdditiveScanCondition03){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/arithmetic_select/select02.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
			CHECK(!parser.hasError())

			VirtualMachine* vm = schem.getVm();
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			const UnicodeString* intoVar = nullptr;
			{
				AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
				AnalyzeStackPopper popper(stackMgr, true);
				stackMgr->addFunctionStack();

				stmt->preAnalyze(actx);
				stmt->analyzeTypeRef(actx);
				stmt->analyze(actx);

				intoVar = stmt->getIntoVar();
				StackVariableAccess* access = stackMgr->findStackVariableAccess(intoVar); __STP(access);
			}

			{
				GcManager* gc = vm->getGc();
				vm->newStack();

				StackPopper* stackPopper = new StackPopper(vm); __STP(stackPopper);
				stmt->init(vm);
				stmt->interpret(vm);
			}

			SelectScanPlanner* planner = stmt->getSelectScanPlanner();
			ConditionsHolder* holder = planner->getConditions();
			RootScanCondition* root = holder->getRoot();

			AbstractScanCondition* cond = root->getCondition();

			EqualityScanCondition* eqcond = dynamic_cast<EqualityScanCondition*>(cond);
			AbstractScanCondition* right = dynamic_cast<AbstractScanCondition*>(eqcond->getRight());

			CHECK(right->hasColumnId() == false);

			{
				AbstractScanCondition* cond2 = right->cloneCondition(); __STP(cond2);
				AbstractCdbValue* val = cond2->evaluate(vm, nullptr, nullptr); __STP(val);
				CdbLongValue* lv = dynamic_cast<CdbLongValue*>(val);

				CHECK(lv->getValue() == 11L);
			}

			right->detectIndexCandidate(vm, planner, nullptr);

			{
				FilterConditionDitector detector(vm, planner);
				right->detectFilterConditions(vm, planner, &detector);
			}
		}
	}
}
