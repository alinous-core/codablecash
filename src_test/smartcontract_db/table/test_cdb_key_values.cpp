/*
 * test_cdb_key_values.cpp
 *
 *  Created on: 2020/05/22
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "engine/CodableDatabase.h"

#include "trx/transaction/CdbTransaction.h"

#include "engine/CdbException.h"
#include "engine/CdbBinaryObject.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"
#include <cstdint>

#include "btreekey/InfinityKey.h"

#include "schema_table/record/table_record_key/CdbByteKey.h"
#include "schema_table/record/table_record_key/CdbShortKey.h"
#include "schema_table/record/table_record_key/CdbIntKey.h"
#include "schema_table/record/table_record_key/CdbLongKey.h"
#include "schema_table/record/table_record_key/CdbStringKey.h"
#include "schema_table/record/table_record_key/CdbRecordKey.h"
#include "schema_table/record/table_record_key/CdbBooleanKey.h"
#include "schema_table/record/table_record_key/CdbNullKey.h"
#include "schema_table/record/table_record_key/CdbInfinityKey.h"

#include "btreekey/NullKey.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"
#include "schema_table/record/table_record_value/CdbNullValue.h"
#include "schema_table/record/table_record_value/CdbInfinityValue.h"


using namespace codablecash;
using namespace alinous;

TEST_GROUP(TestCdbKeyValuesGroup) {
	TEST_SETUP() {
	}
	TEST_TEARDOWN() {
	}
};

TEST(TestCdbKeyValuesGroup, CdbByteKey){
	CdbByteKey key;
	CdbByteKey* key2 = dynamic_cast<CdbByteKey*>(key.clone()); __STP(key2);

	CHECK(key.compareTo(key2) == 0)

	int size = key.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key.toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key.toCdbValue(); __STP(v);
	CdbByteValue* bv = dynamic_cast<CdbByteValue*>(v);

	CHECK(bv != nullptr)
}

TEST(TestCdbKeyValuesGroup, CdbShortKey){
	CdbShortKey key;
	CdbShortKey* key2 = dynamic_cast<CdbShortKey*>(key.clone()); __STP(key2);

	CHECK(key.compareTo(key2) == 0)

	int size = key.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key.toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key.toCdbValue(); __STP(v);
	CdbShortValue* sv = dynamic_cast<CdbShortValue*>(v);

	CHECK(sv != nullptr)
}

TEST(TestCdbKeyValuesGroup, CdbIntKey){
	CdbIntKey key;
	CdbIntKey* key2 = dynamic_cast<CdbIntKey*>(key.clone()); __STP(key2);

	CHECK(key.compareTo(key2) == 0)

	int size = key.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key.toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);
}

TEST(TestCdbKeyValuesGroup, CdbLongKey){
	CdbLongKey key;
	CdbLongKey* key2 = dynamic_cast<CdbLongKey*>(key.clone()); __STP(key2);

	CHECK(key.compareTo(key2) == 0)

	int size = key.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key.toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key.toCdbValue(); __STP(v);
	CdbLongValue* sv = dynamic_cast<CdbLongValue*>(v);

	CHECK(sv != nullptr)
}

TEST(TestCdbKeyValuesGroup, CdbStringKey){
	UnicodeString hello(L"hello");
	CdbStringKey key(&hello);
	CdbStringKey* key2 = dynamic_cast<CdbStringKey*>(key.clone()); __STP(key2);

	CHECK(key.compareTo(key2) == 0)

	int size = key.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key.toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key.toCdbValue(); __STP(v);
	CdbStringValue* sv = dynamic_cast<CdbStringValue*>(v);

	CHECK(sv != nullptr);

	CHECK(key.toInt64() == key2->toInt64());
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey01){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	key->addKey(nullptr);

	CdbRecordKey* key2 = dynamic_cast<CdbRecordKey*>(key->clone()); __STP(key2);

	CHECK(key->compareTo(key2) == 0)

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key->toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	CHECK(key->toInt64() == 1)
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey02){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	key->addKey(new CdbIntKey(1));

	CdbRecordKey* key2 = new CdbRecordKey(); __STP(key2);
	key2->addKey(new CdbIntKey(2));

	CHECK(key->compareTo(key2) < 0)
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey03){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	key->addKey(new CdbIntKey(1));

	CdbRecordKey* key2 = new CdbRecordKey();
	key2->addKey(new CdbIntKey(1));

	CdbRecordKey* key3 = new CdbRecordKey(); __STP(key3);
	key3->addKey(key2);

	CHECK(key->compareTo(key3) == 0)
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey04){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	CdbRecordKey* key2 = new CdbRecordKey(); __STP(key2);

	key->addKey(nullptr);
	key2->addKey(nullptr);

	int result = key->compareTo(key2);
	CHECK(result == 0);
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey05){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	CdbRecordKey* key2 = new CdbRecordKey(); __STP(key2);

	key->addKey(nullptr);
	key2->addKey(new CdbNullKey());

	int result = key->compareTo(key2);
	CHECK(result == 0);
}

TEST(TestCdbKeyValuesGroup, CdbRecordKey06){
	CdbRecordKey* key = new CdbRecordKey(); __STP(key);
	CdbRecordKey* key2 = new CdbRecordKey(); __STP(key2);

	key->addKey(new CdbNullKey());
	key2->addKey(nullptr);

	int result = key->compareTo(key2);
	CHECK(result == 0);
}

TEST(TestCdbKeyValuesGroup, CdbBooleanKey01){
	CdbBooleanKey* key = new CdbBooleanKey(true); __STP(key);

	CdbBooleanKey* key2 = dynamic_cast<CdbBooleanKey*>(key->clone()); __STP(key2);

	CHECK(key->compareTo(key2) == 0)

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key->toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key->toCdbValue(); __STP(v);
	CdbBooleanValue* bv = dynamic_cast<CdbBooleanValue*>(v);

	CHECK(bv != nullptr)
}

TEST(TestCdbKeyValuesGroup, CdbNullKey01){
	CdbNullKey* key = new CdbNullKey(); __STP(key);

	CdbNullKey* key2 = dynamic_cast<CdbNullKey*>(key->clone()); __STP(key2);
	CHECK(key->compareTo(key2) == 0)

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key->toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult < 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult == 0);

	AbstractCdbValue* v = key->toCdbValue(); __STP(v);
	CdbNullValue* bv = dynamic_cast<CdbNullValue*>(v);

	CHECK(bv != nullptr)

	CHECK(key->toInt64() == 0);
}

TEST(TestCdbKeyValuesGroup, CdbInfinityKey01){
	CdbInfinityKey* key = new CdbInfinityKey(); __STP(key);

	CdbInfinityKey* key2 = dynamic_cast<CdbInfinityKey*>(key->clone()); __STP(key2);
	CHECK(key->compareTo(key2) == 0)

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	key->toBinary(buff);

	buff->position(0);

	CdbKeyFactory factory;

	uint32_t type = buff->getInt();
	AbstractBtreeKey* retKey = factory.fromBinary(type, buff);__STP(retKey);

	int cmpResult = retKey->compareTo(key2);
	CHECK(cmpResult == 0);

	InfinityKey inf;
	cmpResult = retKey->compareTo(&inf);
	CHECK(cmpResult == 0);

	NullKey nullkey;
	cmpResult = retKey->compareTo(&nullkey);
	CHECK(cmpResult > 0);

	AbstractCdbValue* v = key->toCdbValue(); __STP(v);
	CdbInfinityValue* bv = dynamic_cast<CdbInfinityValue*>(v);

	CHECK(bv != nullptr)

	CHECK(key->toInt64() == 0);

}


