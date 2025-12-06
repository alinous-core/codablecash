/*
 * ModularProxyObjectInstanceFactory.h
 *
 *  Created on: Dec 3, 2025
 *      Author: iizuka
 */

#ifndef INTER_MODULAR_ACCESS_MODULARPROXYOBJECTINSTANCEFACTORY_H_
#define INTER_MODULAR_ACCESS_MODULARPROXYOBJECTINSTANCEFACTORY_H_

#include "engine/sc_analyze/IVmInstanceFactory.h"

#include "base/ArrayList.h"


namespace alinous {
class CompilationUnit;
class ClassDeclare;
}
using namespace alinous;

namespace codablecash {

class InstanceDependencyContext;

class ModularProxyObjectInstanceFactory : public IVmInstanceFactory {
public:
	ModularProxyObjectInstanceFactory();
	virtual ~ModularProxyObjectInstanceFactory();

	virtual VmClassInstance* createInstance(AnalyzedClass* clazz, VirtualMachine* vm);

	// generate class
	void generateModularClass(UnicodeString* mainFqn, ClassDeclare* ifdec, InstanceDependencyContext* dctx);

private:
	void addCompilantUnit(CompilationUnit* unit) noexcept;

private:
	ArrayList<CompilationUnit>* progs;
};

} /* namespace codablecash */

#endif /* INTER_MODULAR_ACCESS_MODULARPROXYOBJECTINSTANCEFACTORY_H_ */
