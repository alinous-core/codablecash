/*
 * test_cdb_values.cpp
 *
 *  Created on: 2020/05/22
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "engine/CdbException.h"
#include "engine/CdbBinaryObject.h"

#include "schema_table/record/table_record/CdbDataFactory.h"

#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"
#include "schema_table/record/table_record_value/CdbNullValue.h"
#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "base_io/ByteBuffer.h"

#include "vm/VirtualMachine.h"

#include "instance/instance_ref/VmRootReference.h"
#include "instance/instance_ref/PrimitiveReference.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"
#include "schema_table/record/table_record_key/CdbBooleanKey.h"
#include "schema_table/record/table_record_key/CdbNullKey.h"
#include "schema_table/record/table_record_key/CdbInfinityKey.h"

#include "schema_table/record/table_record_value/CdbInfinityValue.h"

#include "schema_table/record/table_record_value/CdbValueList.h"
using namespace codablecash;
using namespace alinous;

TEST_GROUP(TestCdbValuesGroup) {
	TEST_SETUP() {
	}
	TEST_TEARDOWN() {
	}
};

TEST(TestCdbValuesGroup, CdbByteValue){
	CdbByteValue value;
	CdbByteValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);

	CdbByteValue* bv = dynamic_cast<CdbByteValue*>(obj);
	CHECK(bv->getValue() == value2.getValue())
}

TEST(TestCdbValuesGroup, CdbShortValue){
	CdbShortValue value;
	CdbShortValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);

	CdbShortValue* bv = dynamic_cast<CdbShortValue*>(obj);
	CHECK(bv->getValue() == value2.getValue())
}

TEST(TestCdbValuesGroup, CdbIntValue){
	CdbIntValue value;
	CdbIntValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);

	CdbIntValue* bv = dynamic_cast<CdbIntValue*>(obj);
	CHECK(bv->getValue() == value2.getValue())
}

TEST(TestCdbValuesGroup, CdbLongValue){
	CdbLongValue value;
	CdbLongValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);

	CdbLongValue* bv = dynamic_cast<CdbLongValue*>(obj);
	CHECK(bv->getValue() == value2.getValue())
}

TEST(TestCdbValuesGroup, CdbString){
	UnicodeString hello(L"hello");
	CdbStringValue value(&hello);
	CdbStringValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);

	CdbStringValue* bv = dynamic_cast<CdbStringValue*>(obj);
	CHECK(value2.getValue()->equals(bv->getValue()));
}

TEST(TestCdbValuesGroup, CdbString02){
	UnicodeString hello(L"hello");
	CdbStringValue value(&hello);

	CdbStringValue* v = value.limitStringLength(100); __STP(v);

	CHECK(hello.equals(v->getValue()));
}

TEST(TestCdbValuesGroup, CdbString03){
	UnicodeString hello(L"hello");
	CdbStringValue value(&hello);

	CdbStringValue* v = value.limitStringLength(2); __STP(v);

	UnicodeString he(L"he");
	CHECK(he.equals(v->getValue()));
}

TEST(TestCdbValuesGroup, CdbBooleanValue01){
	CdbBooleanValue value(true);
	CdbBooleanValue value2(value);

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);
	CdbBooleanValue* bv = dynamic_cast<CdbBooleanValue*>(obj);

	CHECK(bv->getValue() == value.getValue());

	CdbBooleanValue* value3 = dynamic_cast<CdbBooleanValue*>(value2.copy()); __STP(value3);
	CHECK(value3->getValue() == value.getValue());

	AbstractCdbKey* key = value3->toKey(); __STP(key);
	CdbBooleanKey* bkey = dynamic_cast<CdbBooleanKey*>(key);
	CHECK(bkey != nullptr);
}

TEST(TestCdbValuesGroup, CdbNullValue01){
	CdbNullValue value;
	CdbNullValue* value2 = dynamic_cast<CdbNullValue*>(value.copy()); __STP(value2);

	CHECK(value2->isNull());

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);
	CdbNullValue* bv = dynamic_cast<CdbNullValue*>(obj);

	CHECK(bv->getValue() == value.getValue());

	AbstractCdbKey* key = value.toKey(); __STP(key);
	CdbNullKey* nkey = dynamic_cast<CdbNullKey*>(key);
	CHECK(nkey != nullptr);
}

TEST(TestCdbValuesGroup, CdbInfinityValue01){
	CdbInfinityValue value;
	CdbInfinityValue* value2 = dynamic_cast<CdbInfinityValue*>(value.copy()); __STP(value2);

	CHECK(!value2->isNull());

	int size = value.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	value.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);
	CdbInfinityValue* bv = dynamic_cast<CdbInfinityValue*>(obj);

	CHECK(bv->getValue() == value.getValue());

	AbstractCdbKey* key = value.toKey(); __STP(key);
	CdbInfinityKey* nkey = dynamic_cast<CdbInfinityKey*>(key);
	CHECK(nkey != nullptr);
}

TEST(TestCdbValuesGroup, CdbValueList01){
	CdbValueList list;
	list.addValue(new CdbLongValue(1));

	CdbValueList* value2 = dynamic_cast<CdbValueList*>(list.copy()); __STP(value2);

	int size = list.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	list.toBinary(buff);

	buff->position(0);
	CdbDataFactory factory;

	IBlockObject* obj = factory.makeDataFromBinary(buff); __STP(obj);
	CdbValueList* bv = dynamic_cast<CdbValueList*>(obj);

	AbstractCdbKey* key = bv->toKey(); __STP(key);
	CHECK(key == nullptr);

}

TEST(TestCdbValuesGroup, dataFactoryError){
	int size = 10;
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	buff->put(123);
	buff->position(0);

	CdbDataFactory factory;
	CdbException* ex = nullptr;
	try{
		factory.makeDataFromBinary(buff);
	}catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr)
	delete ex;
}

TEST(TestCdbValuesGroup, toCdbValue01){
	AbstractCdbValue* value = VmInstanceValueConverter::toCdbValue(nullptr, 0);
	CHECK(value == nullptr)
}

TEST(TestCdbValuesGroup, toCdbValue02){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbException* ex = nullptr;
	try{
		VmRootReference* ref = new(vm) VmRootReference(vm); __STP(ref);
		AbstractCdbValue* value = VmInstanceValueConverter::toCdbValue(ref, 0);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestCdbValuesGroup, toCdbValue03){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbException* ex = nullptr;
	try{
		PrimitiveReference* ref = PrimitiveReference::createByteReference(vm, 1); __STP(ref);
		AbstractCdbValue* value = VmInstanceValueConverter::toCdbValue(ref, 0);
	}
	catch(CdbException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestCdbValuesGroup, toCdbValue04){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	PrimitiveReference* ref = PrimitiveReference::createLongReference(vm, 1); __STP(ref);
	AbstractCdbValue* avalue = VmInstanceValueConverter::toCdbValue(ref, AbstractCdbValue::TYPE_BYTE); __STP(avalue);

	CdbByteValue* value = dynamic_cast<CdbByteValue*>(avalue);

	CHECK(value->getValue() == 1)
}

TEST(TestCdbValuesGroup, toCdbValue05){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	PrimitiveReference* ref = PrimitiveReference::createLongReference(vm, 1); __STP(ref);
	AbstractCdbValue* avalue = VmInstanceValueConverter::toCdbValue(ref, AbstractCdbValue::TYPE_SHORT); __STP(avalue);

	CdbShortValue* value = dynamic_cast<CdbShortValue*>(avalue);

	CHECK(value->getValue() == 1)
}

TEST(TestCdbValuesGroup, toCdbValue06){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	PrimitiveReference* ref = PrimitiveReference::createLongReference(vm, 1); __STP(ref);
	AbstractCdbValue* avalue = VmInstanceValueConverter::toCdbValue(ref, AbstractCdbValue::TYPE_LONG); __STP(avalue);

	CdbLongValue* value = dynamic_cast<CdbLongValue*>(avalue);

	CHECK(value->getValue() == 1)
}
