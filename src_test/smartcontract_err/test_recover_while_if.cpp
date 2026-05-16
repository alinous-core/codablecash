/*
 * test_recover_while_if.cpp
 *
 *  Created on: Apr 29, 2026
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "engine/compiler/SmartContractParser.h"

#include "lang/sc_statement/AbstractStatement.h"

#include "base/Exception.h"

#include "alinous_lang/ParseException.h"
#include "engine/sc/CompilationUnit.h"

TEST_GROUP(TestRecoverWhileIfGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestRecoverWhileIfGroup, caseif01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/if01/main_err_if.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse(); __STP(unit);

	CHECK(parser.hasError())
}

TEST(TestRecoverWhileIfGroup, casewhile01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/while01/main_err_while.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse(); __STP(unit);

	CHECK(parser.hasError())
}

TEST(TestRecoverWhileIfGroup, casefor01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_err/resources/for01/main_err_for.alns"))

	SmartContractParser parser(sourceFile);
	CompilationUnit* unit = parser.parse(); __STP(unit);

	CHECK(parser.hasError())
}

