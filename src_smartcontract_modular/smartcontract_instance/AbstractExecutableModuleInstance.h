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
class File;
class SmartContract;
class VirtualMachine;
class CompileError;
class VmClassInstance;
class CompilationUnit;
}
using namespace alinous;

#include "base/ArrayList.h"

namespace codablecash {

class SoftwareVersion;
class ModularInstanceConfig;
class DependencyConfig;
class InstanceDependencyHandler;
class ModularSmartcontractInstance;
class ModuleInstanceClassLoader;

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

	virtual void loadCompilantUnits(const File* projectBaseDir) = 0;
	bool hasCompileError() const noexcept;

	void setMainInstance();
	bool createMainInstance();
	bool interpretInitializer();

	void resetRootReference();

	bool initBeforeAnalyze();
	bool preAnalyze();
	bool preAnalyzeGenerics();
	bool analyzeType();
	bool analyzeMetadata();
	bool analyzeFinal();

	bool loadDependency(ModularSmartcontractInstance* parent);
	bool preAnalyzeDependency();
	bool analyzeTypeDependency();
	bool analyzeDependency();

	void loadLibExportInterfaceUnits(ModuleInstanceClassLoader* clazzloader);

	VirtualMachine* getVM() const noexcept {
		return this->vm;
	}

protected:
	void resetContract();
	void parseSourceFolders(const File *projectBaseDir);
	void scanSourceFolder(File* baseDir, const UnicodeString* folder, const File *projectBaseDir);

	void scanFiles(File* folder, const File *projectBaseDir);
	void addCompilantUnit(File* file, File* base, const File *projectBaseDir);

protected:
	UnicodeString* name;
	UnicodeString* projectRelativePath;

	ArrayList<UnicodeString>* sourceFolders;
	SoftwareVersion* version;

	ModularInstanceConfig* instanceConfig;
	DependencyConfig* dependencyConfig;

protected:
	// SmartContract* contract;
	VirtualMachine* vm;

	VmClassInstance* mainInst;

	const ArrayList<CompileError>* compile_errors;

	InstanceDependencyHandler* dependencyHandler;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_INSTANCE_ABSTRACTEXECUTABLEMODULEINSTANCE_H_ */
