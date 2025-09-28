/*
 * ModularSmartcontractProject.h
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */

#ifndef MODULAR_PROJECT_MODULARSMARTCONTRACTPROJECT_H_
#define MODULAR_PROJECT_MODULARSMARTCONTRACTPROJECT_H_

#include "base/ArrayList.h"

namespace alinous {
class File;
}
using namespace alinous;


namespace codablecash {

class ExecutableSmartcontractModule;
class LibrarySmartcontractModule;

class ModularSmartcontractProject {
public:
	explicit ModularSmartcontractProject(const File* baseDir);
	virtual ~ModularSmartcontractProject();

private:
	File* baseDir;

	ExecutableSmartcontractModule* executableModule;
	ArrayList<LibrarySmartcontractModule>* libModules;
};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_MODULARSMARTCONTRACTPROJECT_H_ */
