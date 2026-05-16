/*
 * test_statement_error.cpp
 *
 *  Created on: Apr 28, 2026
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "engine/compiler/SmartContractParser.h"

#include "lang/sc_statement/AbstractStatement.h"

#include "base/Exception.h"

#include "alinous_lang/ParseException.h"

TEST_GROUP(TestStatementErrorGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestStatementErrorGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(!parser.hasError())
}

TEST(TestStatementErrorGroup, case02_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(parser.hasError())
}

TEST(TestStatementErrorGroup, case03_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err_semicoron.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(parser.hasError())
}

TEST(TestStatementErrorGroup, case04_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err_lbrace.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(parser.hasError())
}

TEST(TestStatementErrorGroup, case05_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err_eof.alns"))

	ParseException* ex = nullptr;
	try{
		SmartContractParser parser(sourceFile);
		CompilationUnit* unit = parser.parse();
	}
	catch(ParseException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestStatementErrorGroup, case06_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err_lex.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(parser.hasError())
}

TEST(TestStatementErrorGroup, case07_err){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/class01/main_err_if.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse();

	CHECK(parser.hasError())
}


