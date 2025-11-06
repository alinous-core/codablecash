/*
 * AbstractReservedClassDeclare.cpp
 *
 *  Created on: 2020/03/17
 *      Author: iizuka
 */

#include "instance/reserved_classes/AbstractReservedClassDeclare.h"
#include "instance/reserved_classes/ReservedClassRegistory.h"

#include "instance/reserved_classes_string/StringClassDeclare.h"

#include "lang/sc_declare/MethodDeclare.h"
#include "lang/sc_declare/MemberVariableDeclare.h"

#include "base/ArrayList.h"
#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang/sc_declare/AccessControlDeclare.h"
#include "lang/sc_declare/ArgumentsListDeclare.h"
#include "lang/sc_declare/ClassExtends.h"
#include "lang/sc_declare/ClassDeclareBlock.h"
#include "lang/sc_declare/ClassImplements.h"

#include "lang/sc_statement/StatementBlock.h"

#include "engine/sc/CompilationUnit.h"

#include "engine/sc_analyze/PackageSpace.h"
#include "engine/sc_analyze/AnalyzedType.h"
#include "engine/sc_analyze/AnalyzeContext.h"
#include "engine/sc_analyze/AnalyzedClass.h"

#include "filestore_block/exceptions.h"

#include "instance/instance_exception/ArrayOutOfBoundsExceptionClassDeclare.h"
#include "instance/instance_exception_class/ExceptionClassDeclare.h"
#include "instance/instance_exception/NullPointerExceptionClassDeclare.h"
#include "instance/instance_exception/TypeCastExceptionClassDeclare.h"
#include "instance/instance_exception/ZeroDivisionExceptionClassDeclare.h"

#include "trx/transaction_exception/DatabaseExceptionClassDeclare.h"

#include "instance/reserved_classes/object/ObjectClassDeclare.h"


namespace alinous {

AbstractReservedClassDeclare::AbstractReservedClassDeclare() : ClassDeclare(RESERVED_CLASS_DECLARE) {
	this->methods = new ArrayList<MethodDeclare>();
	this->members = new ArrayList<MemberVariableDeclare>();
}

AbstractReservedClassDeclare::~AbstractReservedClassDeclare() {
	this->methods->deleteElements();
	delete this->methods;

	this->members->deleteElements();
	delete this->members;

	delete this->extends;
	this->extends = nullptr;
}

ArrayList<MethodDeclare>* AbstractReservedClassDeclare::getMethods() noexcept {
	return this->methods;
}

ArrayList<MemberVariableDeclare>* AbstractReservedClassDeclare::getMemberVariables() noexcept {
	return this->members;
}

void AbstractReservedClassDeclare::preAnalyze(AnalyzeContext* actx) {
	if(this->extends != nullptr){
		this->extends->setParent(this);
		this->extends->preAnalyze(actx);
	}

	int maxLoop = this->methods->size();
	for(int i = 0; i != maxLoop; ++i){
		MethodDeclare* method = this->methods->get(i);

		method->setParent(this);
		method->preAnalyze(actx);
	}

	maxLoop = this->members->size();
	for(int i = 0; i != maxLoop; ++i){
		MemberVariableDeclare* member = this->members->get(i);

		member->setParent(this);
		member->preAnalyze(actx);
	}
}

void AbstractReservedClassDeclare::analyzeTypeRef(AnalyzeContext* actx) {
	if(this->extends != nullptr){
		this->extends->analyzeTypeRef(actx);
	}

	int maxLoop = this->methods->size();
	for(int i = 0; i != maxLoop; ++i){
		MethodDeclare* method = this->methods->get(i);

		method->analyzeTypeRef(actx);
	}

	maxLoop = this->members->size();
	for(int i = 0; i != maxLoop; ++i){
		MemberVariableDeclare* member = this->members->get(i);

		member->analyzeTypeRef(actx);
	}

	PackageSpace* space = actx->getPackegeSpace(this->getPackageName());

	const UnicodeString* fqn = getName();
	AnalyzedClass* dec = space->getClass(fqn);

	// set analyzed class
	if(this->extends != nullptr){
		AnalyzedType* cls = this->extends->getAnalyzedType();
		dec->setExtends(cls->getAnalyzedClass());
	}
}

void AbstractReservedClassDeclare::analyze(AnalyzeContext* actx) {
	int maxLoop = this->methods->size();
	for(int i = 0; i != maxLoop; ++i){
		MethodDeclare* method = this->methods->get(i);

		method->analyze(actx);
	}

	maxLoop = this->members->size();
	for(int i = 0; i != maxLoop; ++i){
		MemberVariableDeclare* member = this->members->get(i);

		member->analyze(actx);
	}
}

void AbstractReservedClassDeclare::addDefaultConstructor(const UnicodeString* className) noexcept {
	MethodDeclare* m = new MethodDeclare();
	this->methods->addElement(m);

	AccessControlDeclare* ctrl = new AccessControlDeclare();
	ctrl->setCtrl(AccessControlDeclare::PUBLIC);
	m->setAccessControl(ctrl);

	UnicodeString* name = new UnicodeString(className);
	m->setName(name);

	ArgumentsListDeclare* argDeclare = new ArgumentsListDeclare();
	m->setArguments(argDeclare);

	StatementBlock* block = new StatementBlock();
	m->setBlock(block);
}

AbstractReservedClassDeclare* AbstractReservedClassDeclare::createFromBinary(ByteBuffer *in) {
	AbstractReservedClassDeclare* ret = nullptr;

	uint16_t classType = in->getShort();

	switch(classType){
	case TYPE_STRING_CLASS:
		ret = new StringClassDeclare();
		break;
	case TYPE_ARRAY_OUT_OF_BOUNDS_EXCEPTION:
		ret = new ArrayOutOfBoundsExceptionClassDeclare();
		break;
	case TYPE_DATABASE_EXCEPTION:
		ret = new DatabaseExceptionClassDeclare();
		break;
	case TYPE_EXCEPTION:
		ret = new ExceptionClassDeclare();
		break;
	case TYPE_NULL_POINTER_EXCEPTION:
		ret = new NullPointerExceptionClassDeclare();
		break;
	case TYPE_TYPE_CAST_EXCEPTION:
		ret = new TypeCastExceptionClassDeclare();
		break;
	case TYPE_ZERO_DIVISION_EXCEPTION:
		ret = new ZeroDivisionExceptionClassDeclare();
		break;
	case TYPE_OBJECT:
		ret = new ObjectClassDeclare();
		break;
	default:
		throw new BinaryFormatException(__FILE__, __LINE__);
	}

	ret->fromBinary(in);

	return ret;
}

int AbstractReservedClassDeclare::binarySize() const {
	checkNotNull(this->block);

	int total = sizeof(uint16_t); // toBinaryHead(out);

	total += sizeof(uint16_t); // out->putShort(getClassType());

	total += stringSize(this->name);

	total += sizeof(uint8_t);
	if(this->extends != nullptr){
		total += this->extends->binarySize();
	}

	total += sizeof(uint8_t);
	if(this->implements != nullptr){
		total += this->implements->binarySize();
	}

	total += sizeof(uint16_t);
	int maxLoop = this->methods->size();
	for(int i = 0; i != maxLoop; ++i){
		MethodDeclare* method = this->methods->get(i);

		total += method->binarySize();
	}

	total += sizeof(uint16_t);
	maxLoop = this->members->size();
	for(int i = 0; i != maxLoop; ++i){
		MemberVariableDeclare* member = this->members->get(i);

		total += member->binarySize();
	}

	return total;
}

void AbstractReservedClassDeclare::toBinary(ByteBuffer *out) const {
	checkNotNull(this->name);

	toBinaryHead(out);
	out->putShort(getClassType());

	putString(out, this->name);

	out->put(this->extends != nullptr ? (uint8_t)1 : (uint8_t)0);
	if(this->extends != nullptr){
		this->extends->toBinary(out);
	}

	out->put(this->implements != nullptr ? (uint8_t)1 : (uint8_t)0);
	if(this->implements != nullptr){
		this->implements->toBinary(out);
	}

	int maxLoop = this->methods->size();
	out->putShort(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		MethodDeclare* method = this->methods->get(i);

		method->toBinary(out);
	}

	maxLoop = this->members->size();
	out->putShort(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		MemberVariableDeclare* member = this->members->get(i);

		member->toBinary(out);
	}
}

void AbstractReservedClassDeclare::fromBinary(ByteBuffer *in) {
	this->name = getString(in);

	bool bl = in->get();
	if(bl == 1){
		CodeElement* element = CodeElement::createFromBinary(in);
		checkKind(element, CodeElement::CLASS_EXTENDS);
		this->extends = dynamic_cast<ClassExtends*>(element);
	}

	bl = in->get();
	if(bl == 1){
		CodeElement* element = CodeElement::createFromBinary(in);
		checkKind(element, CodeElement::CLASS_IMPLEMENTS);
		this->implements = dynamic_cast<ClassImplements*>(element);
	}

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		CodeElement* element = CodeElement::createFromBinary(in); __STP(element);
		MethodDeclare* method = dynamic_cast<MethodDeclare*>(element);

		checkNotNull(method);
		__STP_MV(element);

		this->methods->addElement(method);
	}

	maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		CodeElement* element = CodeElement::createFromBinary(in); __STP(element);
		MemberVariableDeclare* member = dynamic_cast<MemberVariableDeclare*>(element);

		checkNotNull(member);
		__STP_MV(element);

		this->members->addElement(member);
	}
}

void AbstractReservedClassDeclare::addMethod(MethodDeclare *method) noexcept {
	this->methods->addElement(method);
}

ClassDeclare* AbstractReservedClassDeclare::generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) {
	int cap = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);

	toBinary(buff);
	buff->position(0);

	CodeElement* element = CodeElement::createFromBinary(buff);

	return dynamic_cast<ClassDeclare*>(element);
}

} /* namespace alinous */
