/*
 * ModularProxyObjectInstanceFactory.cpp
 *
 *  Created on: Dec 3, 2025
 *      Author: iizuka
 */

#include "inter_modular_access/ModularProxyObjectInstanceFactory.h"
#include "inter_modular_access/ModularProxyClassDeclare.h"

#include "engine/sc_analyze/TypeResolver.h"

#include "engine/sc/CompilationUnit.h"

#include "base/StackRelease.h"

#include "lang/sc_declare/ClassExtends.h"
#include "lang/sc_declare/ClassName.h"

#include "lang/sc_declare/ClassDeclare.h"

#include "lang/sc_declare/ClassImplements.h"
#include "lang/sc_declare/MethodDeclare.h"
#include "lang/sc_declare/PackageDeclare.h"
#include "lang/sc_declare/PackageNameDeclare.h"
#include "lang/sc_declare/ImportsDeclare.h"

#include "inter_modular_access/ModularProxyMethodDeclare.h"
#include "inter_modular_access/ModuleProxyInstance.h"

#include "lang/sc_declare/ImportDeclare.h"


namespace codablecash {

ModularProxyObjectInstanceFactory::ModularProxyObjectInstanceFactory() {
	this->progs = new ArrayList<CompilationUnit>();
}

ModularProxyObjectInstanceFactory::~ModularProxyObjectInstanceFactory() {
	this->progs->deleteElements();
	delete this->progs;
}

VmClassInstance* ModularProxyObjectInstanceFactory::createInstance(AnalyzedClass *clazz, VirtualMachine *vm) {
	ModuleProxyInstance* inst = new(vm) ModuleProxyInstance(clazz, vm);

	return inst;
}

void ModularProxyObjectInstanceFactory::generateModularClass(UnicodeString *mainFqn, ClassDeclare *ifdec, InstanceDependencyContext* dctx) {
	ModularProxyClassDeclare* clazz = new ModularProxyClassDeclare();
	clazz->setFactory(this);
	clazz->setDependencyContext(dctx);

	CompilationUnit* unit = new CompilationUnit();
	unit->addClassDeclare(clazz);
	addCompilantUnit(unit); // Register compilant Unit

	// Unit
	CompilationUnit* originalUnit = ifdec->getCompilationUnit();
	{
		// package
		const UnicodeString* pname = originalUnit->getPackageName();
		PackageDeclare* packageDec = new PackageDeclare();
		PackageNameDeclare* nameDec = new PackageNameDeclare();

		UnicodeString pattern(L"\\.");
		ArrayList<UnicodeString>* segs = pname->split(&pattern); __STP(segs);

		packageDec->setName(nameDec);
		unit->setPackage(packageDec);


		// import
		ImportsDeclare* originalImports = originalUnit->getImportDeclare();
		if(originalImports != nullptr){
			ImportsDeclare* imports = new ImportsDeclare();
			unit->setImports(imports);

			const ArrayList<ImportDeclare>* list = originalImports->getImports();
			int maxLoop = list->size();
			for(int i = 0; i != maxLoop; ++i){
				ImportDeclare* dec = list->get(i);

				imports->addImport(new ImportDeclare(*dec));
			}
		}
	}

	{
		UnicodeString* className = TypeResolver::getClassName(mainFqn);
		className->append(L"Proxy");
		className->append(ifdec->getName());

		clazz->setName(className);
	}

	{
		ClassExtends* extends = new ClassExtends();
		clazz->setExtends(extends);

		ClassName* className = new ClassName();
		extends->setClassName(className);
		className->addStr(mainFqn);
	}

	// interface
	{
		const UnicodeString* fqn = ifdec->getFullQualifiedName();

		ClassImplements* implements = new ClassImplements();
		clazz->setImplements(implements);

		ClassName* className = new ClassName();
		className->addStr(fqn);
		implements->addClassName(className);
	}

	// methods
	{
		ArrayList<MethodDeclare>* list = ifdec->getMethods();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			MethodDeclare* method = list->get(i);

			ModularProxyMethodDeclare* proxy = ModularProxyMethodDeclare::fromMethodDeclare(method);
			clazz->addMethod(proxy);
		}
	}
}

void ModularProxyObjectInstanceFactory::addCompilantUnit(CompilationUnit *unit) noexcept {
	this->progs->addElement(unit);
}

} /* namespace codablecash */
