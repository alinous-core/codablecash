/*
 * test_select_logic_contition.cpp
 *
 *  Created on: 2020/07/31
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base_io/File.h"

#include "vm/VirtualMachine.h"
#include "vm/VmSelectPlannerSetter.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml_parts/SQLWhere.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"
#include "scan_select/scan_planner/base/ConditionsHolder.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"


using namespace alinous;

TEST_GROUP(TestSelectLogicConditionGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestSelectLogicConditionGroup, and01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/logic/where01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SQLWhere* where = lang->sqlWhere(); __STP(where);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		where->preAnalyze(actx);
		where->analyzeTypeRef(actx);
		where->analyze(actx);

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		where->init(vm);

		where->interpret(vm);

		ConditionsHolder* holder = planner->getConditions();
		RootScanCondition* root = holder->getRoot();
		const UnicodeString* str = root->toStringCode();

		UnicodeString sql(L"id = 'test' AND id2 = 100");
		CHECK(sql.equals(str));

		AbstractScanCondition* copy = root->cloneCondition();__STP(copy);
		str = copy->toStringCode();
		CHECK(sql.equals(str));
	}
}

TEST(TestSelectLogicConditionGroup, or01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/logic/where02.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SQLWhere* where = lang->sqlWhere(); __STP(where);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		where->preAnalyze(actx);
		where->analyzeTypeRef(actx);
		where->analyze(actx);

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		where->init(vm);

		where->interpret(vm);

		ConditionsHolder* holder = planner->getConditions();
		RootScanCondition* root = holder->getRoot();
		const UnicodeString* str = root->toStringCode();

		UnicodeString sql(L"id = 'test' OR id2 = 100");
		CHECK(sql.equals(str));

		AbstractScanCondition* copy = root->cloneCondition();__STP(copy);
		str = copy->toStringCode();
		CHECK(sql.equals(str));
	}
}


