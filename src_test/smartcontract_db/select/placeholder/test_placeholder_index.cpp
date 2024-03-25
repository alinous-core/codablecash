/*
 * test_placeholder_index.cpp
 *
 *  Created on: 2023/06/27
 *      Author: iizuka
 */
#include "../../toolkit_index/TestDbSchema1Index01.h"
#include "engine/compiler/SmartContractParser.h"

#include "../../../../src_gen/alinous_lang/AlinousLang.h"
#include "ext_binary/ExtDomArrayObject.h"
#include "test_utils/t_macros.h"

using codablecash::TestDbSchema1Index01;


TEST_GROUP(TestPlaceHolderIndexGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestPlaceHolderIndexGroup, disableIndex01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/placeholder/resources/TestPlaceHolderIndexGroup/disableIndex01.alns"))

	TestDbSchema1Index01 schem(this->env);
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
