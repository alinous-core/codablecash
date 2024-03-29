/*
 * test_scan_columns_part.cpp
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "scan_select/scan_columns/AllScanColumns.h"
#include "scan_select/scan_columns/ScanColumn.h"

#include "vm/VirtualMachine.h"
#include "vm/VmSelectPlannerSetter.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml_parts/SQLSelectTargetList.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "scan_select/scan_columns/ScanColumnHolder.h"

using namespace codablecash;

TEST_GROUP(TestScanColumnsPartGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};


TEST(TestScanColumnsPartGroup, case01){
	AllScanColumns cols;
}

TEST(TestScanColumnsPartGroup, case02){
	ScanColumn col(nullptr);
	const UnicodeString t(L"t");

	col.setAsName(&t);
}

TEST(TestScanColumnsPartGroup, select01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns/select01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"*");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, select02){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns/select02.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"*, public.table1.col1");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, add01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns_arithmetic/select01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"col1 + col2 - col3");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, mul01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns_arithmetic/select02.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"col1 * col2 / col3 % col4");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, params01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns/select03.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"1, 'test', true, false");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, equals01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns_exp/select01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"NOT (col1 = 100)");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, relational01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns_exp/select02.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"col1 > 100 AND col2 >= 100");
		CHECK(str->equals(ans));
	}
}

TEST(TestScanColumnsPartGroup, relational02){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/table_analyze/resources/columns_exp/select03.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		SQLSelectTargetList* selectList = stmt->getSQLSelectTargetList();

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		selectList->preAnalyze(actx);
		selectList->analyzeTypeRef(actx);
		selectList->analyze(actx);


		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		selectList->init(vm);
		selectList->interpret(vm);

		ScanColumnHolder* holder = planner->getColumnHolder();
		UnicodeString* str = holder->toCodeString(); __STP(str);

		UnicodeString ans(L"col1 < 100 OR col2 <= 200");
		CHECK(str->equals(ans));
	}
}

