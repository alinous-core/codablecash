/*
 * test_modular_version_string.cpp
 *
 *  Created on: Oct 13, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "modular_project/ModularSmartcontractVersion.h"

#include "base/UnicodeString.h"
#include "base/Exception.h"


using namespace codablecash;

TEST_GROUP(TestModularVersionStringGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestModularVersionStringGroup, case01) {
	UnicodeString verstr(L"^1.2.3");

	ModularSmartcontractVersion* v = ModularSmartcontractVersion::parseString(&verstr); __STP(v);

	CHECK(v->getVersionType() == ModularSmartcontractVersion::TYPE_CARET);
	CHECK(v->getMajor() == 1);
	CHECK(v->getMinor() == 2);
	CHECK(v->getPatch() == 3);
}

TEST(TestModularVersionStringGroup, case02) {
	UnicodeString verstr(L"~1.2.3");

	ModularSmartcontractVersion* v = ModularSmartcontractVersion::parseString(&verstr); __STP(v);

	CHECK(v->getVersionType() == ModularSmartcontractVersion::TYPE_TILDE);
	CHECK(v->getMajor() == 1);
	CHECK(v->getMinor() == 2);
	CHECK(v->getPatch() == 3);
}

TEST(TestModularVersionStringGroup, case03) {
	UnicodeString verstr(L"~aaa");

	Exception* ex = nullptr;
	try{
		ModularSmartcontractVersion* v = ModularSmartcontractVersion::parseString(&verstr); __STP(v);
	}
	catch(Exception* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

