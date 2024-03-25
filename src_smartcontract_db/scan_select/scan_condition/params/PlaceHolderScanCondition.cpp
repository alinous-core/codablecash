/*
 * PlaceHolderScanCondition.cpp
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/PlaceHolderScanCondition.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang/sc_expression/AbstractExpression.h"

#include "vm/VirtualMachine.h"

#include "instance/AbstractVmInstance.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"
#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbNullValue.h"

#include "instance/instance_gc/StackFloatingVariableHandler.h"

#include "instance/instance_exception/TypeCastExceptionClassDeclare.h"

#include "engine/CdbException.h"

#include "instance/instance_ref/PrimitiveReference.h"

#include "instance/VmInstanceTypesConst.h"
namespace codablecash {

PlaceHolderScanCondition::PlaceHolderScanCondition(const PlaceHolderScanCondition& inst) {
	this->exp = inst.exp;
	this->available = inst.available;
	this->str = nullptr;
}

PlaceHolderScanCondition::PlaceHolderScanCondition(AbstractExpression* exp) {
	this->exp = exp;
	this->available = nullptr;
	this->str = nullptr;
}

PlaceHolderScanCondition::~PlaceHolderScanCondition() {
	resetStr();
}

const UnicodeString* PlaceHolderScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"${}");
	}

	return this->str;
}

void PlaceHolderScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

IValueProvider* PlaceHolderScanCondition::clone() const noexcept {
	return new PlaceHolderScanCondition(*this);
}

AbstractCdbValue* PlaceHolderScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	GcManager* gc = vm->getGc();

	StackFloatingVariableHandler releaser(gc);

	AbstractVmInstance* inst = this->exp->interpret(vm);
	releaser.registerInstance(inst);

	AbstractCdbValue* v = VmInstanceValueConverter::toCdbValue(inst->getInstance(), AbstractCdbValue::TYPE_LONG);

	return v != nullptr ? v : new CdbNullValue();
}

void PlaceHolderScanCondition::setAvailable(AbstractExpression *available) {
	this->available = available;
}

bool PlaceHolderScanCondition::isAvailable(VirtualMachine *vm) const {
	if(this->available != nullptr){
		GcManager* gc = vm->getGc();
		StackFloatingVariableHandler releaser(gc);

		AbstractVmInstance* inst = this->available->interpret(vm);
		releaser.registerInstance(inst);

		if(inst != nullptr && inst->isPrimitive()){
			PrimitiveReference* prref = dynamic_cast<PrimitiveReference*>(inst);
			assert(prref != nullptr);

			uint8_t type = prref->getType();

			if(type == VmInstanceTypesConst::REF_BOOL){
				return prref->getBoolValue();
			}
		}

		throw new CdbException(L"Available needs boolean type and value don't have compatibility.", __FILE__, __LINE__);
	}

	return true;
}

} /* namespace codablecash */
