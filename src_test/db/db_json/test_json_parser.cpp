/*
 * test_json_parser.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "json/JsonHandler.h"
#include "json/JsonTokenizeException.h"
#include "json/JsonTokenizer.h"
#include "json/AbstractJsonToken.h"

#include "json_object/AbstractJsonObject.h"
#include "json_object/JsonObject.h"

#include "json_object/AbstractJsonValue.h"

#include "json_object/JsonValuePair.h"

#include "json_object/JsonBooleanValue.h"

#include "json_object/JsonNumericValue.h"

#include "json/JsonParseException.h"

#include "json/JsonParser.h"
using namespace codablecash;


TEST_GROUP(TestJsonParserGroup) {
	TEST_SETUP() {
		this->env->setup();
	}
	TEST_TEARDOWN() {
		this->env->teardown();
	}
};


TEST(TestJsonParserGroup, parser01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parser01.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	JsonTokenizer* tokenizer = handler.getTokenizer();
	while(!tokenizer->isEndOfFile()){
		AbstractJsonToken* token = tokenizer->getToken(); __STP(token);
	}
}

/**
 * simple json
 */
TEST(TestJsonParserGroup, parser02){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parser02.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	CHECK(dom != nullptr);
}

TEST(TestJsonParserGroup, parser03){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parser03.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	CHECK(dom != nullptr);
	CHECK(dom->getType()  == AbstractJsonObject::JSON_TYPE_OBJECT);

	{
		const JsonValuePair* p0 = dom->get(0);
		AbstractJsonValue* key = p0->getKey();
		AbstractJsonObject* v = p0->getValue();

		CHECK(p0->getType() == AbstractJsonObject::JSON_TYPE_VALUE_PAIR);
		CHECK(v->getType()  == AbstractJsonObject::JSON_TYPE_STRING_VALUE);
	}

	{
		const JsonValuePair* p1 = dom->get(1);
		AbstractJsonValue* key = p1->getKey();
		AbstractJsonObject* v = p1->getValue();

		JsonBooleanValue* value = dynamic_cast<JsonBooleanValue*>(v);
		CHECK(value != nullptr);
		CHECK(v->getType()  == AbstractJsonObject::JSON_TYPE_BOOLEAN_VALUE);
	}

	{
		const JsonValuePair* p2 = dom->get(2);
		AbstractJsonValue* key = p2->getKey();
		AbstractJsonObject* v = p2->getValue();

		JsonNumericValue* value = dynamic_cast<JsonNumericValue*>(v);
		CHECK(value != nullptr);
		CHECK(value->getValue() == 10);
		CHECK(v->getType()  == AbstractJsonObject::JSON_TYPE_NUMERIC_VALUE);
	}
}

TEST(TestJsonParserGroup, array01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/array01.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	const JsonValuePair* p0 = dom->get(0);
	AbstractJsonObject* v = p0->getValue();
	CHECK(v->getType()  == AbstractJsonObject::JSON_TYPE_ARRAY_OBJECT);
}

TEST(TestJsonParserGroup, array02){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/array02.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	const JsonValuePair* p0 = dom->get(0);
	AbstractJsonObject* v = p0->getValue();
	CHECK(v->getType()  == AbstractJsonObject::JSON_TYPE_ARRAY_OBJECT);
}

TEST(TestJsonParserGroup, escape01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/escape01.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);
}

TEST(TestJsonParserGroup, tokenerr01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/tokenerr01.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror01.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr02){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror02.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr03){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror03.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr04){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror04.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr05){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror05.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr06){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror06.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr07){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror07.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr08){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror08.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		AbstractJsonObject* object = handler.parse(); __STP(object);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr09){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror09.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		JsonParser parser(handler.getTokenizer());
		parser.abstractJsonValue();
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, parsererr10){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources/parseerror10.txt"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	Exception* ex = nullptr;
	try{
		JsonParser parser(handler.getTokenizer());
		parser.jsonObject();
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestJsonParserGroup, exceitionCheck01){
	testException<JsonTokenizeException>();
	testException<JsonParseException>();
}
