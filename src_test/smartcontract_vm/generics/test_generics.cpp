/*
 * test_generics.cpp
 *
 *  Created on: Oct 21, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../VmTestUtils.h"

TEST_GROUP(TestGenericsGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestGenericsGroup, case01) {
	const File* projectFolder = this->env->getProjectRoot();
	VmTestUtils util(L"src_test/smartcontract_vm/generics/resources/case01/", projectFolder);

	util.loadAllFiles();
	util.setMain(L"test.fw", L"SmartContract", L"main");

	bool result = util.analyze();
	CHECK(result)

	result = util.createInstance();
	CHECK(result)
}

TEST(TestGenericsGroup, case02) {
	const File* projectFolder = this->env->getProjectRoot();
	VmTestUtils util(L"src_test/smartcontract_vm/generics/resources/case02/", projectFolder);

	bool compile = util.loadAllFiles();
	CHECK(compile);

	util.setMain(L"test.fw", L"SmartContract", L"main");

	bool result = util.analyze();
	CHECK(result)

	result = util.createInstance();
	CHECK(result)
}

TEST(TestGenericsGroup, case03_err) {
	const File* projectFolder = this->env->getProjectRoot();
	VmTestUtils util(L"src_test/smartcontract_vm/generics/resources/case03_err/", projectFolder);

	bool compile = util.loadAllFiles();
	CHECK(compile);

	util.setMain(L"test.fw", L"SmartContract", L"main");

	bool result = util.analyze();
	CHECK(!result)
}

