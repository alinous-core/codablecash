/*
 * test_placeholder.cpp
 *
 *  Created on: 2023/06/26
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit/TestDbSchema02.h"
#include "engine/sc_analyze/AnalyzeContext.h"

#include "engine/sc/CompilationUnit.h"

#include "lang/sc_statement/StatementBlock.h"

#include "engine/compiler/SmartContractParser.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "vm/variable_access/StackVariableAccess.h"

#include "engine/sc_analyze_stack/AnalyzeStackManager.h"

#include "../../toolkit_index/TestDbSchema0Index01.h"
using namespace codablecash;

TEST_GROUP(TestPlaceHolderGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestPlaceHolderGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/placeholder/resources/TestPlaceHolderGroup/case01.alns"))

	TestDbSchema02 schem(this->env);
	schem.init(1024*512);
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		{
			StatementBlock* stmt = lang->statementBlock(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.selectBlock(stmt);
		}

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

TEST(TestPlaceHolderGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/placeholder/resources/TestPlaceHolderGroup/case02.alns"))

	TestDbSchema0Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		{
			StatementBlock* stmt = lang->statementBlock(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.selectBlock(stmt);
		}

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 3);
	}
}

TEST(TestPlaceHolderGroup, case03_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/placeholder/resources/TestPlaceHolderGroup/case03_err.alns"))

	TestDbSchema0Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		{
			StatementBlock* stmt = lang->statementBlock(); __STP(stmt);
			CHECK(!parser.hasError())

			schem.selectBlock(stmt);
		}

		const ExtExceptionObject* ex = schem.getExeptionExObj();
		CHECK(ex != nullptr);
	}
}

