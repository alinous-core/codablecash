/*
 * test_database_equals_checker.cpp
 *
 *  Created on: 2023/07/30
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "schema_table/schema/DatabaseEqualsCheckException.h"
#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "schema_table/record/table_record_key/CdbIntKey.h"
using namespace codablecash;

TEST_GROUP(TestDatabaseEqualsCheckerGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestDatabaseEqualsCheckerGroup, case01){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		DatabaseEqualsChecker::checkLongEquals(1, 2);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case02){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		DatabaseEqualsChecker::checkIntEquals(1, 2);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case03){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		DatabaseEqualsChecker::checkBoolEquals(true, false);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case04){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		CdbOid oid(2L);
		DatabaseEqualsChecker::checkCdbOidEquals(nullptr, &oid);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case05){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		CdbOid oid(1L);
		CdbOid oid2(2L);
		DatabaseEqualsChecker::checkCdbOidEquals(&oid, &oid2);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case06){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		UnicodeString str(L"");
		DatabaseEqualsChecker::checkUnicodeStringEquals(nullptr, &str);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case07){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		UnicodeString str(L"1");
		UnicodeString str2(L"2");
		DatabaseEqualsChecker::checkUnicodeStringEquals(&str, &str2);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case08){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		CdbRecordKey rec;
		DatabaseEqualsChecker::checkRecordEquals(nullptr, &rec);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestDatabaseEqualsCheckerGroup, case09){
	DatabaseEqualsCheckException* ex = nullptr;
	try{
		CdbRecordKey rec;
		rec.addKey(new CdbIntKey(1));
		CdbRecordKey rec2;
		rec2.addKey(new CdbIntKey(2));
		DatabaseEqualsChecker::checkRecordEquals(&rec, &rec2);
	}
	catch(DatabaseEqualsCheckException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}
