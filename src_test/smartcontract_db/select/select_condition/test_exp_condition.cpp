/*
 * test_expcondition.cpp
 *
 *  Created on: 2023/06/17
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "../../toolkit/TestDbSchema02.h"
#include "../../toolkit/TestDbSchema01.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql_dml/SelectStatement.h"

#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/AbstractExtObject.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

#include "scan_select/scan_condition/exp/EqualityScanCondition.h"

#include "scan_select/scan_planner/base/TablesHolder.h"

#include "scan_select/scan_condition/exp/ExpressionListScanCondition.h"

#include "lang_sql/sql_expression/SQLColumnIdentifier.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "scan_select/scan_condition/exp/FunctionCallScanCondition.h"

#include "engine/CdbException.h"

#include "scan_select/scan_condition/exp/RelationalScanCondition.h"
using namespace codablecash;


TEST_GROUP(TestExpConditionGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestExpConditionGroup, EqualityScanCondition01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/exp/EqualityScanCondition01.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 2);

		{
			VirtualMachine* vm = schem.getVm();

			SelectScanPlanner* planner = stmt->getSelectScanPlanner();
			ConditionsHolder* holder = planner->getConditions();
			RootScanCondition* root = holder->getRoot();

			TablesHolder* tableHolder = planner->getTablesHolder();


			EqualityScanCondition* cond = dynamic_cast<EqualityScanCondition*>(root->getCondition());
			IValueProvider* right = cond->getRight();

			FilterConditionDitector detector(vm, planner);
			detector.detect(tableHolder->getScanTarget(0));

			bool res = cond->isFilterable(vm, planner, &detector);
			CHECK(res == true);
		}
	}
}

TEST(TestExpConditionGroup, ExpressionListScanCondition01){
	ExpressionListScanCondition cond;

	SQLColumnIdentifier* sqlId = new SQLColumnIdentifier(); __STP(sqlId);
	ColumnIdentifierScanParam* param = new ColumnIdentifierScanParam(sqlId);

	cond.addElement(param);

	bool bl = cond.isRecordIndependent();
	CHECK(bl == false);
}

TEST(TestExpConditionGroup, FunctionCallScanCondition01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/exp/FunctionCallScanCondition01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		{
			VirtualMachine* vm = schem.getVm();

			SelectScanPlanner* planner = stmt->getSelectScanPlanner();
			ConditionsHolder* holder = planner->getConditions();
			RootScanCondition* root = holder->getRoot();

			TablesHolder* tableHolder = planner->getTablesHolder();


			EqualityScanCondition* cond = dynamic_cast<EqualityScanCondition*>(root->getCondition());
			FunctionCallScanCondition* right = dynamic_cast<FunctionCallScanCondition*>(cond->getLeft());

			FilterConditionDitector detector(vm, planner);
			detector.detect(tableHolder->getScanTarget(0));

			bool res = right->isFilterable(vm, planner, &detector);
			CHECK(res == true);

			right->detectFilterConditions(vm, planner, &detector);

			right->detectIndexCandidate(vm, planner, nullptr);

			res = right->hasColumnId();
			CHECK(res == true);
		}
	}
}

TEST(TestExpConditionGroup, FunctionCallScanCondition02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/exp/FunctionCallScanCondition02.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		{
			VirtualMachine* vm = schem.getVm();

			SelectScanPlanner* planner = stmt->getSelectScanPlanner();
			ConditionsHolder* holder = planner->getConditions();
			RootScanCondition* root = holder->getRoot();

			TablesHolder* tableHolder = planner->getTablesHolder();


			EqualityScanCondition* cond = dynamic_cast<EqualityScanCondition*>(root->getCondition());
			FunctionCallScanCondition* right = dynamic_cast<FunctionCallScanCondition*>(cond->getLeft());

			FilterConditionDitector detector(vm, planner);
			// detector.detect(tableHolder->getScanTarget(0)); // without scan target

			bool res = right->isFilterable(vm, planner, &detector);
			CHECK(res == false);

			right->detectFilterConditions(vm, planner, &detector);

			right->detectIndexCandidate(vm, planner, nullptr);

			res = right->hasColumnId();
			CHECK(res == true);
		}
	}
}

TEST(TestExpConditionGroup, RelationalScanCondition01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/exp/RelationalScanCondition01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);
	}
}

TEST(TestExpConditionGroup, RelationalScanCondition02){
	uint8_t op = 100;
	bool reverse = false;

	CdbException* ex = nullptr;
	try{
		RelationalScanCondition::toIndexType(op, reverse);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}
