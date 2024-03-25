/*
 * test_like_condition.cpp
 *
 *  Created on: 2022/02/10
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "../../toolkit/TestDbSchema03.h"
#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql_dml_parts/SQLWhere.h"

#include "engine/compiler/SmartContractParser.h"
#include "engine/sc_analyze/AnalyzeContext.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "vm/VmSelectPlannerSetter.h"

#include "scan_select/scan_condition/base/RootScanCondition.h"

#include "scan_select/scan_planner/base/ConditionsHolder.h"

#include "lang_sql/sql_dml/SelectStatement.h"

#include "ext_binary/AbstractExtObject.h"

#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"
using alinouslang::AlinousLang;

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestLikeConditionGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};



TEST(TestLikeConditionGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/like/case01/where01.alns"))

	TestDbSchema03 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
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

		where->init(vm);

		SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
		VmSelectPlannerSetter setter(vm, planner);

		where->interpret(vm);

		ConditionsHolder* holder = planner->getConditions();
		RootScanCondition* root = holder->getRoot();
		const UnicodeString* str = root->toStringCode();

		UnicodeString sql(L"test_table.name LIKE '%naka%' ESCAPE '%'");
		CHECK(sql.equals(str));

		root->analyzeConditions(vm, planner);

		bool bl = root->isRecordIndependent();
		CHECK(!bl);

		bl = root->isAvailable(vm);
		CHECK(bl);
	}
}

TEST(TestLikeConditionGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/like/case02/select01.alns"))

	TestDbSchema03 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email_id: 11}");
		CHECK(str->equals(ans0));
	}
}

TEST(TestLikeConditionGroup, case03_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_condition/resources/conditions/like/case03/select01.alns"))

	TestDbSchema03 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(parser.hasError())
	}
}

