/*
 * VmInstanceValueConverter.cpp
 *
 *  Created on: 2020/06/17
 *      Author: iizuka
 */

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "instance/IAbstractVmInstanceSubstance.h"
#include "instance/VmInstanceTypesConst.h"

#include "instance/instance_ref/PrimitiveReference.h"

#include "instance/instance_string/VmStringInstance.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbShortValue.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "engine/CdbException.h"


namespace codablecash {

AbstractCdbValue* VmInstanceValueConverter::toCdbValue(IAbstractVmInstanceSubstance* substance, uint8_t targetCdbColumnType) {
	if(substance == nullptr){
		return nullptr;
	}

	uint8_t type = substance->getInstType();
	AbstractCdbValue* value = nullptr;

	PrimitiveReference* pref = nullptr;
	VmStringInstance* strInst = nullptr;

	switch(type){
	case VmInstanceTypesConst::INST_STRING:
		strInst = dynamic_cast<VmStringInstance*>(substance);
		value = new CdbStringValue(strInst->toString());
		break;
	case VmInstanceTypesConst::REF_BOOL:
	case VmInstanceTypesConst::REF_BYTE:
	case VmInstanceTypesConst::REF_CHAR:
	case VmInstanceTypesConst::REF_SHORT:
	case VmInstanceTypesConst::REF_INT:
	case VmInstanceTypesConst::REF_LONG:
		pref = dynamic_cast<PrimitiveReference*>(substance);
		value = fromPrimitiveToCdbValue(pref, targetCdbColumnType);
		break;
	default:
		throw new CdbException(L"Column type and value don't have compatibility.", __FILE__, __LINE__);
		break;
	}

	return value;
}

AbstractCdbValue* VmInstanceValueConverter::fromPrimitiveToCdbValue(PrimitiveReference* pref, uint8_t targetCdbColumnType) {
	AbstractCdbValue* value = nullptr;

	switch(targetCdbColumnType){
	case AbstractCdbValue::TYPE_BYTE:
		value = new CdbByteValue(pref->getByteValue());
		break;
	case AbstractCdbValue::TYPE_SHORT:
		value = new CdbShortValue(pref->getShortValue());
		break;
	case AbstractCdbValue::TYPE_INT:
		value = new CdbIntValue(pref->getIntValue());
		break;
	case AbstractCdbValue::TYPE_LONG:
		value = new CdbLongValue(pref->getLongValue());
		break;
	default:
		throw new CdbException(L"Column type and value don't have compatibility.", __FILE__, __LINE__);
		break;
	}

	return value;
}

IAbstractVmInstanceSubstance* VmInstanceValueConverter::toVmInstance(VirtualMachine* vm, AbstractCdbValue* cdbValue) {
	if(cdbValue == nullptr){
		return nullptr;
	}
	IAbstractVmInstanceSubstance* ret = nullptr;

	uint8_t type = cdbValue->getType();
	switch(type){
	case AbstractCdbValue::TYPE_BYTE:
	{
		CdbByteValue* byteValue = dynamic_cast<CdbByteValue*>(cdbValue);
		int8_t value = byteValue->getValue();
		PrimitiveReference* ref = PrimitiveReference::createByteReference(vm, value);

		ret = ref;
		break;
	}
	case AbstractCdbValue::TYPE_SHORT:
	{
		CdbShortValue* shortValue = dynamic_cast<CdbShortValue*>(cdbValue);
		int16_t value = shortValue->getValue();
		PrimitiveReference* ref = PrimitiveReference::createShortReference(vm, value);

		ret = ref;
		break;
	}
	case AbstractCdbValue::TYPE_INT:
	{
		CdbIntValue* intValue = dynamic_cast<CdbIntValue*>(cdbValue);
		int32_t value = intValue->getValue();
		PrimitiveReference* ref = PrimitiveReference::createIntReference(vm, value);

		ret = ref;
		break;
	}
	case AbstractCdbValue::TYPE_LONG:
	{
		CdbLongValue* longValue = dynamic_cast<CdbLongValue*>(cdbValue);
		int64_t value = longValue->getValue();
		PrimitiveReference* ref = PrimitiveReference::createLongReference(vm, value);

		ret = ref;
		break;
	}
	case AbstractCdbValue::TYPE_STRING:
	{
		CdbStringValue* stringValue = dynamic_cast<CdbStringValue*>(cdbValue);
		const UnicodeString* value = stringValue->getValue();
		VmStringInstance* vmString = new(vm) VmStringInstance(vm, value);

		ret = vmString;
		break;
	}
	case AbstractCdbValue::TYPE_BOOLEAN:
	{
		CdbBooleanValue* blValue = dynamic_cast<CdbBooleanValue*>(cdbValue);
		bool bl = blValue->getValue();

		PrimitiveReference* ref = PrimitiveReference::createBoolReference(vm, bl ? 1 : 0);
		ret = ref;
		break;
	}
	case AbstractCdbValue::TYPE_NULL:
	default:
		break;
	}

	return ret;
}


} /* namespace codablecash */
