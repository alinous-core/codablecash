/*
 * test_value_converter.cpp
 *
 *  Created on: 2021/12/08
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"
#include "instance/instance_ref/PrimitiveReference.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"

#include "instance/VmInstanceTypesConst.h"

#include "schema_table/record/table_record_value/CdbNullValue.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"
using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestValueConverterGroup) {
	TEST_SETUP() {
	}
	TEST_TEARDOWN() {
	}
};


TEST(TestValueConverterGroup, toVmInstance01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbByteValue value(10);
	IAbstractVmInstanceSubstance* sub = VmInstanceValueConverter::toVmInstance(vm, &value); __STP(sub);

	PrimitiveReference* pr = dynamic_cast<PrimitiveReference*>(sub);
	int8_t v = pr->getByteValue();
	CHECK(pr->getType() == VmInstanceTypesConst::REF_BYTE)

	CHECK(v == 10);
}

TEST(TestValueConverterGroup, toVmInstance02){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbShortValue value(10);
	IAbstractVmInstanceSubstance* sub = VmInstanceValueConverter::toVmInstance(vm, &value); __STP(sub);

	PrimitiveReference* pr = dynamic_cast<PrimitiveReference*>(sub);
	int8_t v = pr->getShortValue();
	CHECK(pr->getType() == VmInstanceTypesConst::REF_SHORT)

	CHECK(v == 10);
}

TEST(TestValueConverterGroup, toVmInstance03){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbNullValue value;
	IAbstractVmInstanceSubstance* sub = VmInstanceValueConverter::toVmInstance(vm, &value); __STP(sub);
	CHECK(sub == nullptr);
}

TEST(TestValueConverterGroup, toVmInstance04){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	CdbBooleanValue value(true);
	IAbstractVmInstanceSubstance* sub = VmInstanceValueConverter::toVmInstance(vm, &value); __STP(sub);
	PrimitiveReference* pr = dynamic_cast<PrimitiveReference*>(sub);

	CHECK(pr->getType() == VmInstanceTypesConst::REF_BOOL)
	bool v = pr->getBoolValue();
	CHECK(v == true);
}
