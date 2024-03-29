/*
 * DomVariableInstance.cpp
 *
 *  Created on: 2020/06/22
 *      Author: iizuka
 */

#include "instance/instance_dom/DomVariableInstance.h"
#include "instance/instance_dom/DomVariableReference.h"
#include "instance/instance_dom/DomRuntimeReference.h"

#include "instance/VmInstanceTypesConst.h"

#include "instance/instance_gc/GcManager.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "engine/sc_analyze/AnalyzedType.h"

#include "instance/instance_parts/VMemList.h"

#include "instance/instance_ref/AbstractReference.h"

#include "ext_binary/ExtDomObject.h"

#include "instance/instance_string/VmStringInstance.h"

#include "instance/instance_parts/VMemHashMapKeySet.h"
namespace alinous {

DomVariableInstance::DomVariableInstance(VirtualMachine* vm) : AbstractDomInstance(vm, VmInstanceTypesConst::INST_DOM) {
	this->properties = new(vm) VMemHashmap<VmStringInstance, DomRuntimeReference>(vm);
	this->list = new(vm) VMemList<AbstractReference>(vm);
	this->str = nullptr;
}

DomVariableInstance::~DomVariableInstance() {
	delete this->properties;
	delete this->list;
	delete this->str;
}

void DomVariableInstance::removeInnerRefs(GcManager* gc) noexcept {
	Iterator<VmStringInstance>* it = this->properties->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const VmStringInstance* key = it->next();
		DomRuntimeReference* ref = this->properties->get(key);

		gc->removeObject(ref);
		ref->resetOnGc();

		delete ref;
	}

	this->properties->clear();
}

IAbstractVmInstanceSubstance* DomVariableInstance::getInstance() noexcept {
	return this;
}

AbstractReference* DomVariableInstance::wrap(IAbstractVmInstanceSubstance* owner, VirtualMachine* vm) {
	DomVariableReference* ref = new(vm) DomVariableReference(owner, vm, this);

	return ref;
}

uint8_t DomVariableInstance::getInstType() const noexcept {
	return getType();
}

AnalyzedType DomVariableInstance::getRuntimeType() const noexcept {
	return AnalyzedType(AnalyzedType::TYPE_DOM);
}

const VMemList<AbstractReference>* DomVariableInstance::getReferences() const noexcept {
	this->list->reset();

	Iterator<VmStringInstance>* it = this->properties->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const VmStringInstance* key = it->next();
		DomRuntimeReference* ref = this->properties->get(key);

		this->list->addElement(ref);
	}


	return this->list;
}

AbstractExtObject* DomVariableInstance::toClassExtObject(const UnicodeString* name, VTableRegistory* reg) {
	ExtDomObject* domExobj = new ExtDomObject(name);

	Iterator<VmStringInstance>* it = this->properties->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const VmStringInstance* key = it->next();
		DomRuntimeReference* ref = this->properties->get(key);

		const UnicodeString* keyStr = key->toString();
		AbstractExtObject* exobj = ref->toClassExtObject(keyStr, reg);

		domExobj->put(keyStr, exobj);
	}

	return domExobj;
}

const VMemList<AbstractReference>* DomVariableInstance::getInstReferences() const noexcept {
	return getReferences();
}

int DomVariableInstance::instHashCode() const noexcept {
	return hashCode();
}

bool DomVariableInstance::instIsPrimitive() const noexcept {
	return false;
}

bool DomVariableInstance::instIsNull() const noexcept {
	return isNull();
}

int DomVariableInstance::instValueCompare(const IAbstractVmInstanceSubstance* right) const noexcept {
	return valueCompare(right);
}

AbstractExtObject* DomVariableInstance::instToClassExtObject(const UnicodeString* name, VTableRegistory* table) {
	return toClassExtObject(name, table);
}

const UnicodeString* DomVariableInstance::toString() const noexcept {
	delete this->str;
	this->str = new UnicodeString(L"{");

	bool first = true;
	Iterator<VmStringInstance>* it = this->properties->keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const VmStringInstance* vmstr = it->next();
		AbstractReference* ref = this->properties->get(vmstr);

		if(first){
			first = false;
		}else{
			this->str->append(L", ");
		}

		const UnicodeString* key = vmstr->toString();
		this->str->append(key);

		this->str->append(L" : ");

		const UnicodeString* refstr = ref != nullptr ? ref->toString() : &AbstractReference::NULL_STR;

		if(ref != nullptr && !ref->isDom() && !ref->isPrimitive()){
			this->str->append(L"\"");
		}

		this->str->append(refstr);

		if(ref != nullptr && !ref->isDom() && !ref->isPrimitive()){
			this->str->append(L"\"");
		}
	}

	this->str->append(L"}");

	return this->str;
}

int DomVariableInstance::valueCompare(const IAbstractVmInstanceSubstance* right) const noexcept {
	const DomVariableInstance* inst = dynamic_cast<const DomVariableInstance*>(right);

	int64_t diff = ((int64_t)this) - ((int64_t)inst);
	if(diff == 0){
		return 0;
	}

	return diff > 0 ? 1 : -1;
}

void DomVariableInstance::putProperty(VirtualMachine* vm, const VmStringInstance* key, IAbstractVmInstanceSubstance* substance) noexcept {
	GcManager* gc = vm->getGc();

	DomRuntimeReference* rr = new(vm) DomRuntimeReference(this, vm);
	rr->substitute(substance, vm);

	DomRuntimeReference* lastrr = this->properties->put(key, rr);
	if(lastrr != nullptr){
		gc->removeObject(lastrr);
		delete lastrr;
	}
}

DomRuntimeReference* DomVariableInstance::getProperty(VirtualMachine* vm, const VmStringInstance* key) noexcept {
	return this->properties->get(key);
}

bool DomVariableInstance::hasProperty(const VmStringInstance* name) {
	VMemHashMapKeySet<VmStringInstance, DomRuntimeReference>* keys = this->properties->keySet();

	return keys->hasKey(name);
}

} /* namespace alinous */
