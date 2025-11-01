/*
 * test_generics_class.cpp
 *
 *  Created on: Oct 18, 2025
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "engine/compiler/SmartContractParser.h"

#include "lang/sc_expression_literal/NullLiteral.h"
#include "lang/sc_declare/GenericsClassDeclare.h"

#include "alinous_lang/AlinousLang.h"

#include "base/StackRelease.h"

TEST_GROUP(TestGenericsClassGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestGenericsClassGroup, case01) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	ClassDeclare* clazz = lang->classDeclare(); __STP(clazz);

	CHECK(!parser.hasError())

	GenericsClassDeclare* gclazz = dynamic_cast<GenericsClassDeclare*>(clazz);
	CHECK(gclazz != nullptr);

	int total = gclazz->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	gclazz->toBinary(buff);

	buff->position(0);

	GenericsClassDeclare* gclass2 = dynamic_cast<GenericsClassDeclare*>(CodeElement::createFromBinary(buff)); __STP(gclass2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	gclass2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}

TEST(TestGenericsClassGroup, case02) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics02.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	ClassDeclare* clazz = lang->classDeclare(); __STP(clazz);

	CHECK(!parser.hasError())

	GenericsClassDeclare* gclazz = dynamic_cast<GenericsClassDeclare*>(clazz);
	CHECK(gclazz != nullptr);

	int total = gclazz->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	gclazz->toBinary(buff);

	buff->position(0);

	GenericsClassDeclare* gclass2 = dynamic_cast<GenericsClassDeclare*>(CodeElement::createFromBinary(buff)); __STP(gclass2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	gclass2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}

TEST(TestGenericsClassGroup, case02_1) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics02_1.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	ClassDeclare* clazz = lang->classDeclare(); __STP(clazz);

	CHECK(!parser.hasError())

	GenericsClassDeclare* gclazz = dynamic_cast<GenericsClassDeclare*>(clazz);
	CHECK(gclazz != nullptr);

	int total = gclazz->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	gclazz->toBinary(buff);

	buff->position(0);

	GenericsClassDeclare* gclass2 = dynamic_cast<GenericsClassDeclare*>(CodeElement::createFromBinary(buff)); __STP(gclass2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	gclass2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}

TEST(TestGenericsClassGroup, case03) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics03.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractType* typeDec = lang->typeDeclare(); __STP(typeDec);
	CHECK(!parser.hasError())

	GenericsObjectType* genericType = dynamic_cast<GenericsObjectType*>(typeDec);
	CHECK(genericType != nullptr);

	int total = genericType->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	genericType->toBinary(buff);

	buff->position(0);

	GenericsObjectType* genericType2 = dynamic_cast<GenericsObjectType*>(CodeElement::createFromBinary(buff)); __STP(genericType2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	genericType2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}

TEST(TestGenericsClassGroup, case04) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics04.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractType* typeDec = lang->typeDeclare(); __STP(typeDec);
	CHECK(!parser.hasError())

	GenericsObjectType* genericType = dynamic_cast<GenericsObjectType*>(typeDec);
	CHECK(genericType != nullptr);

	int total = genericType->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	genericType->toBinary(buff);

	buff->position(0);

	GenericsObjectType* genericType2 = dynamic_cast<GenericsObjectType*>(CodeElement::createFromBinary(buff)); __STP(genericType2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	genericType2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}

TEST(TestGenericsClassGroup, case05) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics05.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractType* typeDec = lang->typeDeclare(); __STP(typeDec);
	CHECK(!parser.hasError())

	GenericsObjectType* genericType = dynamic_cast<GenericsObjectType*>(typeDec);
	CHECK(genericType != nullptr);

	int total = genericType->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	genericType->toBinary(buff);

	buff->position(0);

	GenericsObjectType* genericType2 = dynamic_cast<GenericsObjectType*>(CodeElement::createFromBinary(buff)); __STP(genericType2);
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(total, true); __STP(buff2);
	genericType2->toBinary(buff2);

	int cmp = buff->binaryCmp(buff2);
	CHECK(cmp == 0);
}


TEST(TestGenericsClassGroup, case06) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics06.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	CompilationUnit* unit = lang->compilationUnit(); __STP(unit);
	CHECK(!parser.hasError())
}

TEST(TestGenericsClassGroup, case07) {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/generics/generics07.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractStatement* stmt = lang->statement(); __STP(stmt);
	CHECK(!parser.hasError())
}

