/*
 * test_strict_equals.cpp
 *
 *  Created on: 2023/07/26
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbInfinityValue.h"
#include "schema_table/record/table_record_value/CdbNullValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"
#include "schema_table/record/table_record_value/CdbOidValueList.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"
#include "schema_table/record/table_record_value/CdbValueList.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/ScanRecordIndentity.h"

#include "engine/CdbOid.h"
using namespace codablecash;

TEST_GROUP(TestStrictEqualsGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestStrictEqualsGroup, CdbBooleanValue01){
	CdbBooleanValue bl(true);
	CdbBooleanValue bl2(true);

	bool res = bl.strictEquals(&bl2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbByteValue01){
	CdbByteValue v(1);
	CdbByteValue v2(1);

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbLongValue01){
	CdbLongValue v;
	CdbLongValue v2;

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbShortValue01){
	CdbShortValue v;
	CdbShortValue v2;

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbInfinityValue01){
	CdbInfinityValue v;
	CdbInfinityValue v2;

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbNullValue01){
	CdbNullValue v;
	CdbNullValue v2;

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbOidValueList01){
	CdbOidValueList v;
	CdbOidValueList v2;

	v.addOid(new CdbOid(1L));
	v2.addOid(new CdbOid(1L));

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbOidValueList02){
	CdbOidValueList v;
	CdbOidValueList v2;

	v.addOid(new CdbOid(1L));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbOidValueList03){
	CdbOidValueList v;
	CdbOidValueList v2;

	v.addOid(new CdbOid(1L));
	v.addOid(new CdbOid(2L));
	v2.addOid(new CdbOid(1L));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbOidValueList04){
	CdbOidValueList v;
	CdbOidValueList v2;

	v.addOid(new CdbOid(1L));
	v.addOid(new CdbOid(2L));
	v2.addOid(new CdbOid(1L));
	v2.addOid(new CdbOid(3L));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbValueList01){
	CdbValueList v;
	CdbValueList v2;

	v.addValue(new CdbIntValue(1));
	v2.addValue(new CdbIntValue(1L));

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbValueList02){
	CdbValueList v;
	CdbValueList v2;

	v.addValue(new CdbIntValue(1));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbValueList03){
	CdbValueList v;
	CdbValueList v2;

	v.addValue(new CdbIntValue(1));
	v.addValue(new CdbIntValue(2));
	v2.addValue(new CdbIntValue(1));
	v2.addValue(new CdbIntValue(3));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbRecord01){
	CdbRecord v;
	CdbRecord v2;

	v.setOid(1L);
	v2.setOid(1L);

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbRecord02){
	CdbRecord v;
	CdbRecord v2;

	v.setOid(1L);
	v2.setOid(2L);

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbRecord03){
	CdbRecord v;
	CdbRecord v2;

	v.setOid(1L);
	v2.setOid(1L);

	v.addValue(new CdbIntValue(1L));
	v2.addValue(new CdbIntValue(1L));

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, CdbRecord04){
	CdbRecord v;
	CdbRecord v2;

	v.setOid(1L);
	v2.setOid(1L);

	v.addValue(new CdbIntValue(1L));
	v2.addValue(new CdbIntValue(1L));
	v2.addValue(new CdbIntValue(2L));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, CdbRecord05){
	CdbRecord v;
	CdbRecord v2;

	v.setOid(1L);
	v2.setOid(1L);

	v.addValue(new CdbIntValue(1));
	v.addValue(new CdbIntValue(2));
	v2.addValue(new CdbIntValue(1));
	v2.addValue(new CdbIntValue(3));

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, ScanRecordIndentity01){
	ScanRecordIndentity v;
	ScanRecordIndentity v2;

	bool res = v.strictEquals(&v2);
	CHECK(res == true)
}

TEST(TestStrictEqualsGroup, ScanRecordIndentity02){
	ScanRecordIndentity v;
	ScanRecordIndentity v2;

	CdbOid oid1(1L);
	v.addOid(&oid1);

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

TEST(TestStrictEqualsGroup, ScanRecordIndentity03){
	ScanRecordIndentity v;
	ScanRecordIndentity v2;

	CdbOid oid1(1L);
	CdbOid oid2(2L);
	CdbOid oid3(3L);

	v.addOid(&oid1);
	v.addOid(&oid2);

	v2.addOid(&oid1);
	v2.addOid(&oid3);

	bool res = v.strictEquals(&v2);
	CHECK(res == false)
}

