/*
 * AbstractReservedMethodDeclare.cpp
 *
 *  Created on: Nov 2, 2025
 *      Author: iizuka
 */

#include "instance/reserved_classes/AbstractReservedMethodDeclare.h"

#include "instance/instance_ref/ObjectReference.h"

#include "lang/sc_declare/ArgumentsListDeclare.h"
#include "lang/sc_declare_types/AbstractType.h"

#include "base/exceptions.h"

#include "engine/sc_analyze/AnalyzeContext.h"
#include "engine/sc_analyze/AnalyzedClass.h"
#include "engine/sc_analyze/AnalyzedThisClassStackPopper.h"

#include "vm/VirtualMachine.h"

#include "vm/variable_access/FunctionArguments.h"

#include "vm/stack/VmStack.h"
#include "vm/stack/MethodArgumentSetupper.h"
#include "vm/stack/StackPopper.h"


namespace alinous {

AbstractReservedMethodDeclare::AbstractReservedMethodDeclare(uint32_t methodId) : MethodDeclare(RESERVED_METHOD_DECLARE) {
	this->methodId = methodId;
}

AbstractReservedMethodDeclare::~AbstractReservedMethodDeclare() {

}

AbstractReservedMethodDeclare* AbstractReservedMethodDeclare::createMethodFromBinary(ByteBuffer *in) {
	uint16_t methodId = in->getShort();

	switch(methodId){
	default:
		return nullptr;
	}

}

void AbstractReservedMethodDeclare::preAnalyze(AnalyzeContext *actx) {
	AnalyzedClass* aclass = actx->getAnalyzedClass(this);
	aclass->addMemberMethodDeclare(this);

	this->args->setParent(this);
	this->args->preAnalyze(actx);

	if(this->type != nullptr){
		this->type->preAnalyze(actx);
		this->type->setParent(this);
		actx->detectGenericsType(this->type);
	}
}

void AbstractReservedMethodDeclare::analyzeTypeRef(AnalyzeContext *actx) {
	this->args->analyzeTypeRef(actx);

	if(this->type != nullptr){
		this->type->analyzeTypeRef(actx);
	}
}

void AbstractReservedMethodDeclare::analyze(AnalyzeContext *actx) {
	this->args->analyze(actx);

	AnalyzedClass* aclass = actx->getAnalyzedClass(this);
	AnalyzedThisClassStackPopper thisStack(actx, aclass);

	if(this->type != nullptr){
		this->type->analyze(actx);
	}
}

void AbstractReservedMethodDeclare::init(VirtualMachine *vm) {
}

void AbstractReservedMethodDeclare::interpret(FunctionArguments *args, VirtualMachine *vm) {
	MethodArgumentSetupper argSetup(args, vm);
	{
		vm->newStack();
		StackPopper stackPopper(vm);

		interpretFunctionArguments(vm);
	}
}

void AbstractReservedMethodDeclare::interpretFunctionArguments(VirtualMachine *vm) {
	FunctionArguments* args = vm->getFunctionArguments();
	VmStack* stack = vm->topStack();

	if(!isStatic()){
		VmClassInstance* _this = args->getThisPtr();
		ObjectReference* ref = ObjectReference::createObjectReference(stack, _this, vm);

		stack->addInnerReference(ref);
	}

	const ArrayList<IAbstractVmInstanceSubstance>* list = args->getArguments();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		IAbstractVmInstanceSubstance* inst = list->get(i);

		if(inst != nullptr){
			stack->addInnerReference(inst->wrap(stack, vm));
		}
		else{
			ObjectReference* ref = ObjectReference::createObjectReference(stack, nullptr, vm);
			stack->addInnerReference(ref);
		}
	}
}

MethodDeclare* AbstractReservedMethodDeclare::generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) const {
	int cap = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);

	toBinary(buff);
	buff->position(0);

	CodeElement* element = CodeElement::createFromBinary(buff);

	return dynamic_cast<AbstractReservedMethodDeclare*>(element);
}

} /* namespace alinous */
