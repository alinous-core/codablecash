/*
 * GenericsTypeRefChecker.cpp
 *
 *  Created on: Oct 30, 2025
 *      Author: iizuka
 */

#include "engine/sc_analyze/GenericsTypeRefChecker.h"

#include "lang/sc_declare_types/GenericsObjectType.h"
#include "lang/sc_declare_types/AbstractType.h"

#include "base/UnicodeString.h"


namespace alinous {

GenericsTypeRefChecker::GenericsTypeRefChecker(TypeResolver* resolver) {
	this->resolver = resolver;
	this->stack = new ArrayList<const UnicodeString>();
	this->hasError = false;
	this->errorTypeName = nullptr;
}

GenericsTypeRefChecker::~GenericsTypeRefChecker() {
	this->resolver = nullptr;
	delete this->stack;
}

void GenericsTypeRefChecker::visit(AbstractType *type) {
	if(!this->hasError){
		const UnicodeString* typeName = type->toString();
		if(AbstractType::TYPE_GENERICS_OBJECT == type->getKind()){
			GenericsObjectType* gtype = dynamic_cast<GenericsObjectType*>(type);
			typeName = gtype->getRawClassName();
		}

		if(hasClass(typeName)){
			this->hasError = true;
			this->errorTypeName = typeName;
			return;
		}

		this->stack->addElement(typeName);
	}
}

void GenericsTypeRefChecker::exit(AbstractType *type) {
	if(!this->hasError){
		this->stack->pop();
	}
}

bool GenericsTypeRefChecker::hasClass(const UnicodeString* className) const noexcept {
	bool ret = false;

	int maxLoop = this->stack->size();
	for(int i = 0; i != maxLoop; ++i){
		const UnicodeString* name = this->stack->get(i);

		if(className->equals(name)){
			ret = true;
			break;
		}
	}

	return ret;
}

} /* namespace alinous */
