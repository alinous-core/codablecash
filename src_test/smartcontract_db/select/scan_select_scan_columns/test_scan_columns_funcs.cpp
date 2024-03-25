/*
 * test_scan_columns_funcs.cpp
 *
 *  Created on: 2023/06/05
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit/TestDbSchema02.h"
#include "engine/compiler/SmartContractParser.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "alinous_lang/AlinousLang.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "ext_binary/AbstractExtObject.h"

#include "ext_binary/ExtStringClass.h"

#include "engine/sc_analyze_stack/AnalyzeStackPopper.h"

#include "vm/VirtualMachine.h"

#include "engine/sc_analyze_stack/AnalyzeStackManager.h"
using namespace codablecash;


TEST_GROUP(TestScanColumnsFuncsGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestScanColumnsFuncsGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/func/upper01/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.execSelectStmt(stmt);
		}

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		exobj = array->get(0);
		CHECK(exobj != nullptr);

		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		AbstractExtObject* col = domObj->get(L"col0");
		CHECK(col != nullptr);

		ExtStringClass* strClass = dynamic_cast<ExtStringClass*>(col);
		UnicodeString ans(L"TANAKA@TANAKA.COM");

		CHECK(ans.equals(strClass->getValue()));
	}
}

/**
 * Null
 */
TEST(TestScanColumnsFuncsGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/func/upper02/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);
	}

	const AbstractExtObject* exobj = schem.getExtResultObj();
	const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

	int size = array->size();
	CHECK(size == 1);

	exobj = array->get(0);
	CHECK(exobj != nullptr);

	const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
	AbstractExtObject* col = domObj->get(L"col0");
	CHECK(col->isNull());
}

/**
 * arg error
 */
TEST(TestScanColumnsFuncsGroup, case03){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/scan_select_scan_columns/resources/func/upper03_err/select01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		{
			VirtualMachine* vm = schem.getVm();
			AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
			actx->setVm(vm);

			AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
			AnalyzeStackPopper popper(stackMgr, true);
			stackMgr->addFunctionStack();

			stmt->preAnalyze(actx);
			stmt->analyzeTypeRef(actx);
			stmt->analyze(actx);

			bool err = actx->hasError();
			CHECK(err == true);
		}
	}
}
