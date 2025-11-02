/*
 * test_json_parser2.cpp
 *
 *  Created on: Oct 12, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "json/JsonHandler.h"

#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/JsonArrayObject.h"


using namespace codablecash;


TEST_GROUP(TestJsonParser2Group) {
	TEST_SETUP() {
		this->env->setup();
	}
	TEST_TEARDOWN() {
		this->env->teardown();
	}
};

TEST(TestJsonParser2Group, parser01){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources02/blankarray.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	CHECK(dom != nullptr);
	CHECK(dom->getType()  == AbstractJsonObject::JSON_TYPE_OBJECT);

	const JsonValuePair* pair = dom->get(L"ar");
	AbstractJsonObject* value = pair->getValue();
	JsonArrayObject* arrayObject = dynamic_cast<JsonArrayObject*>(value);

	CHECK(arrayObject != nullptr);
}

TEST(TestJsonParser2Group, parser02){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources02/config.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	CHECK(dom != nullptr);
	CHECK(dom->getType()  == AbstractJsonObject::JSON_TYPE_OBJECT);
}

TEST(TestJsonParser2Group, parser03){
	const File* projectFolder = this->env->getProjectRoot();
	File* file = projectFolder->get(L"src_test/db/db_json/resources02/blankobject.json"); __STP(file);

	JsonHandler handler;
	handler.loadFile(file, 4);

	AbstractJsonObject* object = handler.parse(); __STP(object);
	JsonObject* dom = dynamic_cast<JsonObject*>(object);

	CHECK(dom != nullptr);
	CHECK(dom->getType()  == AbstractJsonObject::JSON_TYPE_OBJECT);
}
