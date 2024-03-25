/*
 * test_grouped_records.cpp
 *
 *  Created on: 2022/01/06
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"

#include "base_io/ByteBuffer.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestGroupRecordsGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestGroupRecordsGroup, case01){
	CdbRecord rec;
	CHECK(!rec.isGrouped());
}

TEST(TestGroupRecordsGroup, case02){
	CdbRecord recempty;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(20));

	CdbGroupedRecord base(&rec, nullptr, nullptr, nullptr);
	CdbGroupedRecord* grec = new CdbGroupedRecord(base); __STP(grec);

	int total = grec->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);

	grec->toBinary(buff);
	buff->position(0);

	uint8_t type = buff->get();
	CHECK(type == AbstractCdbValue::TYPE_RECORD);

	CdbGroupedRecord* grec2 = new CdbGroupedRecord(&recempty, nullptr, nullptr, nullptr); __STP(grec2);
	grec2->fromBinary(buff);

	const ArrayList<AbstractCdbValue>* list = grec2->getValues();
	CHECK(list->size() == 1);

	AbstractCdbValue* v = list->get(0);
	CdbIntValue* iv = dynamic_cast<CdbIntValue*>(v);
	CHECK(iv->getValue() == 20);
}
