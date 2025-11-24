/*
 * ModularSmartcontractInstance.h
 *
 *  Created on: Sep 22, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_INSTANCE_MODULARSMARTCONTRACTINSTANCE_H_
#define SMARTCONTRACT_INSTANCE_MODULARSMARTCONTRACTINSTANCE_H_

#include "base/HashMap.h"
#include "base/ArrayList.h"

namespace alinous {
class File;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class ModularProjectConfig;
class ExecutableModuleInstance;
class LibraryExectableModuleInstance;

class ModularSmartcontractInstance {
public:
	ModularSmartcontractInstance();
	virtual ~ModularSmartcontractInstance();

	void setModularProjectConfig(const ModularProjectConfig* config);
	void setExecutableModuleInstance(ExecutableModuleInstance* inst);
	void addLibraryModuleInstance(const UnicodeString* name, LibraryExectableModuleInstance* inst);

	bool libraryExists(const UnicodeString* name) const noexcept;
	LibraryExectableModuleInstance* getLibraryModuleInstance(const UnicodeString* modName) const noexcept;

	void loadCompilantUnits(const File* projectBaseDir);
	bool hasCompileError() const noexcept;

	bool analyze();
	void setMainInstance();
	bool createMainInstance();
	bool interpretInitializer();

	void resetRootReference();

private:
	void initBeforeAnalyze();
	void preAnalyze();
	void preAnalyzeGenerics();
	void analyzeType();
	void analyzeMetadata();
	void analyzeFinal();

	// dependency
	void loadDependency();
	void preAnalyzeDependency();
	void analyzeTypeDependency();
	void analyzeDependency();

private:
	ModularProjectConfig* config;

	ExecutableModuleInstance* execModule;

	HashMap<UnicodeString, LibraryExectableModuleInstance>* libraries;
	ArrayList<LibraryExectableModuleInstance>* libArray;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_INSTANCE_MODULARSMARTCONTRACTINSTANCE_H_ */
