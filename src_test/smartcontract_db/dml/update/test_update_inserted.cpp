/*
 * test_update_inserted.cpp
 *
 *  Created on: 2022/01/15
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "engine/compiler/SmartContractParser.h"
#include "alinous_lang/AlinousLang.h"

#include "lang_sql/sql_dml/UpdateStatement.h"
#include "lang_sql/sql_dml/InsertStatement.h"

#include "../../toolkit/TestDbSchema01.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "vm/VirtualMachine.h"


using namespace codablecash;

static const wchar_t* checkRecord(const ExtDomArrayObject* array, int pos){
	const AbstractExtObject* exobj = array->get(pos);

	const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
	const UnicodeString* str = domObj->toString();

	return str->towString();
}


TEST_GROUP(TestUpdateInsertedStatementGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUpdateInsertedStatementGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, insertFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/insert.alns"))
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/sql.alns"))
	_ST(File, selectFile, projectFolder->get(L"src_test/smartcontract_db/dml/update/resources/insert/case01/select.alns"))

	TestDbSchema01 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();

	schem.begin();
	{
		SmartContractParser parser(insertFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		InsertStatement* stmt = lang->insertStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execInsertStmt(stmt);
	}
	{
		/**
		UPDATE test_table
		SET
			name = 'tanaka3'
		WHERE
			id = 1000;
		 */
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		UpdateStatement* stmt = lang->updateStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execUpdateStmt(stmt);
	}
	schem.commit();

	{
		/**
		 *	SELECT * INTO res FROM test_table
			WHERE
			name = 'tanaka3';
		 */
		SmartContractParser parserSelect(selectFile);
		AlinousLang* lang = parserSelect.getDebugAlinousLang();

		SelectStatement* sstmt = lang->selectStatement(); __STP(sstmt);
		CHECK(!parserSelect.hasError())

		schem.execSelectStmt(sstmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		const wchar_t* ch = checkRecord(array, 0);
		UnicodeString ans(L"{name: 'tanaka3', id: 1000, email_id: 100}");
		CHECK(ans.equals(ch));
	}
}


