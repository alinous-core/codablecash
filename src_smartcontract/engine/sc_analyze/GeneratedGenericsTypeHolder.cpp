/*
 * GenericsTypeHolder.cpp
 *
 *  Created on: Oct 24, 2025
 *      Author: iizuka
 */

#include "engine/sc_analyze/GeneratedGenericsTypeHolder.h"
#include "engine/sc_analyze/AnalyzeContext.h"
#include "engine/sc_analyze/ValidationError.h"
#include "engine/sc_analyze/TypeResolver.h"
#include "engine/sc_analyze/AnalyzedType.h"
#include "engine/sc_analyze/AnalyzedClass.h"

#include "lang/sc_declare/GenericsGeneratedClassDeclare.h"
#include "lang/sc_declare/GenericsClassDeclare.h"
#include "lang/sc_declare/GenericsParameter.h"
#include "lang/sc_declare/ImportDeclare.h"
#include "lang/sc_declare/ClassDeclare.h"
#include "lang/sc_declare/PackageNameDeclare.h"
#include "lang/sc_declare/ImportsDeclare.h"

#include "lang/sc_declare_types/GenericsObjectType.h"
#include "lang/sc_declare_types/ObjectType.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"
#include "base/HashMap.h"

#include "vm/exceptions.h"

#include "engine/sc/CompilationUnit.h"


namespace alinous {

GeneratedGenericsTypeHolder::GeneratedGenericsTypeHolder() {
	this->list = new ArrayList<CompilationUnit>();
	this->map = new HashMap<UnicodeString, CompilationUnit>();

	this->inputList = new ArrayList<GenericsObjectType>();
}

GeneratedGenericsTypeHolder::~GeneratedGenericsTypeHolder() {
	delete this->map;

	this->list->deleteElements();
	delete this->list;

	delete this->inputList;
}

void GeneratedGenericsTypeHolder::addAbstractTypeCandidate(GenericsObjectType *candidate) {
	this->inputList->addElement(candidate);
}

void GeneratedGenericsTypeHolder::generateGeneratedGenericsClassDeclare(AnalyzeContext *actx) {
	TypeResolver* resolver = actx->getTypeResolver();

	int maxLoop = this->inputList->size();
	for(int i = 0; i != maxLoop; ++i){
		GenericsObjectType* gtype = this->inputList->get(i);

		handleGenericsObjectType(gtype, actx);
	}
}

void GeneratedGenericsTypeHolder::handleGenericsObjectType(GenericsObjectType* gtype, AnalyzeContext *actx) {
	const UnicodeString* canonicalName = gtype->getClassName();
	registerGeneratedClassDeclare(canonicalName, gtype, actx);
}

void GeneratedGenericsTypeHolder::registerGeneratedClassDeclare(const UnicodeString *canonicalName, GenericsObjectType *gtype, AnalyzeContext *actx) {
	CompilationUnit* gunit = nullptr;
	TypeResolver* resolver = actx->getTypeResolver();
	CompilationUnit* classdec = getGenericsGeneratedClassDeclare(canonicalName);

	if(classdec == nullptr){
		const UnicodeString* name = gtype->getRawClassName();
		AnalyzedType* atype = resolver->findClassType(gtype, name); __STP(atype);

		AnalyzedClass* aclass = atype->getAnalyzedClass();
		ClassDeclare* clazz = aclass->getClassDeclare();
		GenericsClassDeclare* gclass = dynamic_cast<GenericsClassDeclare*>(clazz);
		if(gclass == nullptr){
			actx->addValidationError(ValidationError::CODE_CLASS_MEMBER_DOES_NOT_EXISTS, gtype, L"The class {0} is not generics type.", {name} );
			return;
		}

		ArrayList<GenericsParameter>* paramList = gclass->getParams();
		ArrayList<AbstractType>* typelist = gtype->getGenericTypes();


		if(typelist->size() != paramList->size()){
			actx->addValidationError(ValidationError::CODE_WRONG_GENERICS_PARAMS, gtype, L"The generics class {0} has different number of params.", {name} );
			return;
		}

		// make input
		HashMap<UnicodeString, AbstractType> input;
		ArrayList<ImportDeclare> importList;
		importList.setDeleteOnExit();

		int maxLoop = typelist->size();
		for(int i = 0; i != maxLoop; ++i){
			GenericsParameter* param = paramList->get(i);
			AbstractType* type = typelist->get(i);

			// handle generics
			if(type->isGenericsType()){
				GenericsObjectType* gnericsType = dynamic_cast<GenericsObjectType*>(type);
				handleGenericsObjectType(gnericsType, actx);
			}

			const UnicodeString* key = param->getGenericsName();
			input.put(key, type);

			AnalyzedType* analyzedtype = resolver->resolveType(type, type); __STP(analyzedtype);
			if(analyzedtype == nullptr){
				const UnicodeString* name = type->toString();
				actx->addValidationError(ValidationError::CODE_WRONG_CLASS_NAME, gtype, L"The generics parameter class {0} can't be resolved.", {name} );
				return;
			}

			// add import
			AnalyzedClass* aclass = analyzedtype->getAnalyzedClass();
			const UnicodeString* fqn = aclass->getFullQualifiedName();

			ImportDeclare* imdec = new ImportDeclare();
			imdec->setClassName(new UnicodeString(fqn));

			importList.addElement(imdec);
		}

		// generate CompilationUnit
		CompilationUnit* unit = gclass->getCompilationUnit();
		gunit = unit->generateGenericsImplement(&input);

		// add import
		ImportsDeclare* importDeclare = gunit->getImportDeclare();
		maxLoop = importList.size();
		for(int i = 0; i != maxLoop; ++i){
			ImportDeclare* im = importList.get(i);
			ImportDeclare* imdec = new ImportDeclare(*im);

			imdec->setParent(importDeclare);
			importDeclare->addImport(imdec);

			imdec->preAnalyze(actx);
		}

		this->list->addElement(gunit);
		this->map->put(canonicalName, gunit);
	}

	{
		// change import
		CompilationUnit* unit = gtype->getCompilationUnit();
		ImportsDeclare* imports = unit->getImportDeclare();

		// last name
		UnicodeString fromfqn(L"");

		const UnicodeString* name = gtype->getRawClassName();
		AnalyzedType* atype = resolver->findClassType(gtype, name); __STP(atype);

		if(atype != nullptr){ // only first time.
			AnalyzedClass* aclass = atype->getAnalyzedClass();
			ClassDeclare* clazz = aclass->getClassDeclare();
			GenericsClassDeclare* gclass = dynamic_cast<GenericsClassDeclare*>(clazz);

			const UnicodeString* fqn = gclass->getFullQualifiedName();
			fromfqn.append(fqn);


			// new name
			const UnicodeString* packageName = gclass->getPackageName();
			const UnicodeString* gclassName = gtype->getClassName();

			UnicodeString tofqn(L"");
			if(packageName != nullptr){
				tofqn.append(packageName);
				tofqn.append(L".");
			}
			tofqn.append(gclassName);

			imports->replaceImport(&fromfqn, &tofqn);
		}
	}

	// preAnalyze
	if(classdec == nullptr){
		gunit->preAnalyze(actx);
	}
}

CompilationUnit* GeneratedGenericsTypeHolder::getGenericsGeneratedClassDeclare(const UnicodeString *canonicalName) const noexcept {
	return this->map->get(canonicalName);
}

void GeneratedGenericsTypeHolder::preAnalyzeGenerics(AnalyzeContext *actx) {
	/*int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		CompilationUnit* unit = this->list->get(i);
		unit->preAnalyze(actx);
	}*/
}

void GeneratedGenericsTypeHolder::analyzeTypeRefGenerics(AnalyzeContext *actx) {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		CompilationUnit* unit = this->list->get(i);
		unit->analyzeType(actx);
	}
}

void GeneratedGenericsTypeHolder::analyzeGenerics(AnalyzeContext *actx) {
	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		CompilationUnit* unit = this->list->get(i);
		unit->analyze(actx);
	}
}

} /* namespace alinous */
