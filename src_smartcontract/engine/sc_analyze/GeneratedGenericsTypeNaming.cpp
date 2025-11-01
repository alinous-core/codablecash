/*
 * GeneratedGenericsTypeNaming.cpp
 *
 *  Created on: Oct 27, 2025
 *      Author: iizuka
 */

#include "engine/sc_analyze/GeneratedGenericsTypeNaming.h"

#include "lang/sc_declare_types/GenericsObjectType.h"
#include "lang/sc_declare/GenericsClassDeclare.h"
#include "lang/sc_declare/GenericsParameter.h"

#include "base/UnicodeString.h"


namespace alinous {

UnicodeString* GeneratedGenericsTypeNaming::fromGenericsObjectType(const GenericsObjectType *objectRefType) {
	UnicodeString* ret = new UnicodeString(PREFIX);

	{
		const UnicodeString* clazzname = objectRefType->getRawClassName();
		ret->append(clazzname);
		ret->append(L"_");
	}

	ArrayList<AbstractType>* list = objectRefType->getGenericTypes();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractType* type = list->get(i);
		ObjectType* objectType = dynamic_cast<ObjectType*>(type);

		const UnicodeString* name = objectType->getClassName();

		ret->append(L"_");
		ret->append(name);
	}

	return ret;
}

UnicodeString* GeneratedGenericsTypeNaming::fromGenericsClassDeclare(const GenericsClassDeclare *gclazz,
		HashMap<UnicodeString, AbstractType> *input) {
	UnicodeString* ret = new UnicodeString(PREFIX);

	{
		const UnicodeString* clazzname = gclazz->getName();
		ret->append(clazzname);
		ret->append(L"_");
	}

	ArrayList<GenericsParameter>* list = gclazz->getParams();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		GenericsParameter* param = list->get(i);
		const UnicodeString* paramName = param->getGenericsName();

		AbstractType* type = input->get(paramName);
		assert(type != nullptr);

		ObjectType* objectType = dynamic_cast<ObjectType*>(type);

		const UnicodeString* name = objectType->getClassName();

		ret->append(L"_");
		ret->append(name);
	}

	return ret;
}


} /* namespace alinous */
