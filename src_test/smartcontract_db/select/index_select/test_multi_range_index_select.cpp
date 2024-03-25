/*
 * test_multi_range_select.cpp
 *
 *  Created on: 2023/06/23
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../../toolkit_index/TestDbSchema2Index01.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql_dml/SelectStatement.h"

using namespace codablecash;

TEST_GROUP(TestMultiRangeIndexSelectGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestMultiRangeIndexSelectGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/index_select/resources/index/case01.alns"))

	TestDbSchema2Index01 schem(this->env);
	schem.init(1024*512);
	schem.insert01();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);
	}
}

