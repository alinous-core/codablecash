/*
 * test_sc_declare_types.cpp
 *
 *  Created on: 2020/03/04
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "vm/VirtualMachine.h"
#include "engine/sc/SmartContract.h"
#include "base_io_stream/FileInputStream.h"

#include "engine/sc_analyze/AnalyzedType.h"

#include "base/UnicodeString.h"

#include "../VmTestUtils.h"
#include "engine/sc_analyze/AnalyzeContext.h"

#include "lang/sc_declare_types/ObjectType.h"
#include "engine/sc/CompilationUnit.h"

#include "base/Exception.h"

#include "lang/sc_declare_types/BoolType.h"
#include "lang/sc_declare_types/ByteType.h"
#include "lang/sc_declare_types/CharType.h"
#include "lang/sc_declare_types/ShortType.h"
#include "lang/sc_declare_types/IntType.h"
#include "lang/sc_declare_types/LongType.h"
#include "lang/sc_declare_types/VoidType.h"
#include "lang/sc_declare_types/StringType.h"

#include "lang/sc_declare/PackageNameDeclare.h"

#include "lang/sc_declare_types/DomType.h"
#include "TestVisitor.h"

using namespace alinous;

TEST_GROUP(TestScDeclareTypesGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestScDeclareTypesGroup, BoolType){
	BoolType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&BoolType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, ByteType){
	ByteType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&ByteType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, CharType){
	CharType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&CharType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, ShortType){
	ShortType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&ShortType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, IntType){
	IntType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&IntType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, LongType){
	LongType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&LongType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, VoidType){
	VoidType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&VoidType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, StringType){
	StringType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&StringType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, DomType){
	DomType type;

	const UnicodeString* str = type.toString();
	CHECK(str->equals(&DomType::TYPE_NAME));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}

TEST(TestScDeclareTypesGroup, ObjectType){
	ObjectType type;

	PackageNameDeclare *pkg = new PackageNameDeclare();
	pkg->addSegment(new UnicodeString(L"test"));
	pkg->addSegment(new UnicodeString(L"fw"));
	type.setPackageName(pkg);

	UnicodeString *clazzName = new UnicodeString(L"SmartContract");
	type.setName(clazzName);

	const UnicodeString* str = type.toString();

	UnicodeString ans(L"test.fw.SmartContract");
	CHECK(ans.equals(str));

	TestVisitor visitor;
	type.visit(&visitor);
	CHECK(visitor.getCount() == 0);
}



