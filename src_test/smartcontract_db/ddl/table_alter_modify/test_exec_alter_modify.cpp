/*
 * test_exec_alter_modify.cpp
 *
 *  Created on: 2020/09/13
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "smartcontract_db/toolkit/TestDbSchema01.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter01.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter02.h"
#include "smartcontract_db/toolkit_alter//TestDbSchemaAlter03.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "engine/CodableDatabase.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTableIndex.h"

#include "lang_sql/sql_expression/SQLWildCard.h"

TEST_GROUP(TestExecAlterMofdifyGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};


// ALTER TABLE test_table MODIFY email_id varchar(255) not null unique default 0;
TEST(TestExecAlterMofdifyGroup, case01){
	TestDbSchema01 tester(this->env);
	tester.init();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == true);
	CHECK(col->isNotnull() == true);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index->isUnique());

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx != nullptr);
}

/**
 *
 * not unique -> unique
 * ALTER TABLE test_table MODIFY email_id int not null unique default '0';
 */
TEST(TestExecAlterMofdifyGroup, case02){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case02.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == true);
	CHECK(col->isNotnull() == true);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index->isUnique());

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx != nullptr);
}

/**
 *
 * no change
 * ALTER TABLE test_table MODIFY email_id int default '0';
 */
TEST(TestExecAlterMofdifyGroup, case03){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case03.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == false);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

/*
 * Release unique
 * ALTER TABLE test_table MODIFY email_id int default '0';
 */
TEST(TestExecAlterMofdifyGroup, case04){
	TestDbSchemaAlter02 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case03.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == false);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

/**
 * Release not null
 * ALTER TABLE test_table MODIFY name text default '';
 */
TEST(TestExecAlterMofdifyGroup, case05){
	TestDbSchemaAlter02 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case05.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"name");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == false);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"name");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"name");
	CHECK(idx == nullptr);
}

/**
 * Release unique
 * ALTER TABLE test_table MODIFY name text default '';
 */
TEST(TestExecAlterMofdifyGroup, case06){
	TestDbSchemaAlter03 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case05.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"name");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == false);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"name");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"name");
	CHECK(idx == nullptr);
}

/**
 * Add unique
 * ALTER TABLE test_table MODIFY email_id int unique default 0;
 */
TEST(TestExecAlterMofdifyGroup, case07){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case07.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == true);
	CHECK(col->isNotnull() == false);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index->isUnique());

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx != nullptr);
}

/**
 * handle not null with default no change
 * ALTER TABLE test_table MODIFY email_id int not null default 0;
 */
TEST(TestExecAlterMofdifyGroup, case08){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert02();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case08.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == true);

	const UnicodeString* strdef = col->getDefaultValue();
	UnicodeString defans(L"0");
	CHECK(strdef->equals(defans));

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

/**
 * handle not null with default change
 * ALTER TABLE test_table MODIFY email_id int not null default 1;
 */
TEST(TestExecAlterMofdifyGroup, case09){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert02();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case09.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == true);

	const UnicodeString* strdef = col->getDefaultValue();
	UnicodeString defans(L"1");
	CHECK(strdef->equals(defans));

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

/**
 * handle not null with default -> null
 * ALTER TABLE test_table MODIFY email_id int not null;
 */
TEST(TestExecAlterMofdifyGroup, case10){
	TestDbSchemaAlter01 tester(this->env);
	tester.init(1024*10);
	tester.insert02();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case10.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == true);

	const UnicodeString* strdef = col->getDefaultValue();
	CHECK(strdef == nullptr);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

/**
 * default null
 * ALTER TABLE test_table MODIFY email_id int DEFAULT NULL;
 */
TEST(TestExecAlterMofdifyGroup, case11){
	TestDbSchemaAlter02 tester(this->env);
	tester.init(1024*10);
	tester.insert01();

	VirtualMachine* vm = tester.getVm();

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter_modify/resources/exec_alter/case12.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
		actx->setVm(vm);

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		stmt->interpret(vm);
	}

	CdbTableColumn* col = tester.getColumn(L"test_table", L"email_id");
	CHECK(col->isUnique() == false);
	CHECK(col->isNotnull() == false);

	const UnicodeString* strdef = col->getDefaultValue();
	CHECK(strdef == nullptr);

	CdbTableIndex* index = tester.getIndex(L"test_table", L"email_id");
	CHECK(index == nullptr);

	IndexStore* idx = tester.getIndexStore(L"test_table", L"email_id");
	CHECK(idx == nullptr);
}

