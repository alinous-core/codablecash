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

	void loadCompilantUnits(const File* projectBaseDir);

private:
	ModularProjectConfig* config;

	ExecutableModuleInstance* execModule;

	HashMap<UnicodeString, LibraryExectableModuleInstance>* libraries;
	ArrayList<LibraryExectableModuleInstance>* libArray;

};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_INSTANCE_MODULARSMARTCONTRACTINSTANCE_H_ */
