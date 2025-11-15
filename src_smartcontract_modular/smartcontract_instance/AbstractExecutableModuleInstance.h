/*
 * AbstractExecutableModule.h
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_INSTANCE_ABSTRACTEXECUTABLEMODULEINSTANCE_H_
#define SMARTCONTRACT_INSTANCE_ABSTRACTEXECUTABLEMODULEINSTANCE_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

#include "base/ArrayList.h"

namespace codablecash {

class SoftwareVersion;
class ModularInstanceConfig;
class DependencyConfig;

class AbstractExecutableModuleInstance {
public:
	AbstractExecutableModuleInstance();
	virtual ~AbstractExecutableModuleInstance();

	void setName(const UnicodeString* name) noexcept;
	void setProjectRelativePath(const UnicodeString* path) noexcept;
	void addSourceFolders(const UnicodeString* folder);
	void setSoftwareVersion(const SoftwareVersion* v);
	void setInstanceConfig(const ModularInstanceConfig* config);
	void setDependencyConfig(const DependencyConfig* config);

protected:
	UnicodeString* name;
	UnicodeString* projectRelativePath;

	ArrayList<UnicodeString>* sourceFolders;
	SoftwareVersion* version;

	ModularInstanceConfig* instanceConfig;
	DependencyConfig* dependencyConfig;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_INSTANCE_ABSTRACTEXECUTABLEMODULEINSTANCE_H_ */
