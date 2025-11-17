/*
 * AbstractExecutableModule.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */
#include "smartcontract_instance/AbstractExecutableModuleInstance.h"

#include "modular_project/ModularInstanceConfig.h"
#include "modular_project/DependencyConfig.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"
#include "base/ArrayList.h"

#include "base_io/ByteBuffer.h"
#include "base_io/File.h"

#include "base_io_stream/exceptions.h"
#include "base_io_stream/FileInputStream.h"

#include "bc/ExceptionThrower.h"
#include "bc/SoftwareVersion.h"

#include "engine/sc/SmartContract.h"

#include "engine/compiler/CompileError.h"

#include "vm/VirtualMachine.h"

#include "json_object/AbstractJsonValue.h"

#include "ext_arguments/AbstractFunctionExtArguments.h"

#include "instance/instance_gc/GcManager.h"


namespace codablecash {

AbstractExecutableModuleInstance::AbstractExecutableModuleInstance() {
	this->name = nullptr;
	this->projectRelativePath = nullptr;

	this->sourceFolders = new ArrayList<UnicodeString>();
	this->version = nullptr;
	this->instanceConfig = nullptr;
	this->dependencyConfig = nullptr;

	this->contract = nullptr;
	this->vm = nullptr;
	this->compile_errors = nullptr;
	this->mainInst = nullptr;
}

AbstractExecutableModuleInstance::~AbstractExecutableModuleInstance() {
	delete this->name;
	delete this->projectRelativePath;

	this->sourceFolders->deleteElements();
	delete this->sourceFolders;

	delete this->version;
	delete this->instanceConfig;
	delete this->dependencyConfig;

	vm->destroy();
	delete this->vm;

	this->contract = nullptr;

	this->mainInst = nullptr;
}

void AbstractExecutableModuleInstance::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(name);
}

void AbstractExecutableModuleInstance::setProjectRelativePath(const UnicodeString *path) noexcept {
	delete this->projectRelativePath;
	this->projectRelativePath = new UnicodeString(path);
}

void AbstractExecutableModuleInstance::addSourceFolders(const UnicodeString *folder) {
	this->sourceFolders->addElement(new UnicodeString(folder));
}

void AbstractExecutableModuleInstance::setSoftwareVersion(const SoftwareVersion *v) {
	delete this->version;
	this->version = new SoftwareVersion(*v);
}

void AbstractExecutableModuleInstance::setInstanceConfig(const ModularInstanceConfig *config) {
	delete this->instanceConfig;

	int cap = config->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	config->toBinary(buff);

	buff->position(0);
	this->instanceConfig = ModularInstanceConfig::createFromBinary(buff);
}

void AbstractExecutableModuleInstance::setDependencyConfig(const DependencyConfig *config) {
	delete this->dependencyConfig;
	this->dependencyConfig = new DependencyConfig(*config);
}

void AbstractExecutableModuleInstance::resetContract() {
	delete this->contract;
	this->contract = new SmartContract();

	delete this->vm;
	this->vm = new VirtualMachine(1024*1024);

	this->compile_errors = nullptr;
}

void AbstractExecutableModuleInstance::parseSourceFolders(const File *projectBaseDir) {
	File* baseDir = projectBaseDir->get(this->projectRelativePath); __STP(baseDir);

	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->exists(), L"Module base folder does not exists.", __FILE__, __LINE__);
	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->isDirectory(), L"Module base folder id not a directory.", __FILE__, __LINE__);

	int maxLoop = this->sourceFolders->size();
	for(int i = 0; i != maxLoop; ++i){
		const UnicodeString* folder = this->sourceFolders->get(i);
		scanSourceFolder(baseDir, folder);
	}
}

void AbstractExecutableModuleInstance::scanSourceFolder(File *baseDir, const UnicodeString *folder) {
	File* dir = baseDir->get(folder); __STP(dir);
	scanFiles(dir);

	this->compile_errors = this->contract->getCompileErrors();
}

void AbstractExecutableModuleInstance::scanFiles(File *folder) {
	ArrayList<UnicodeString>* filesList = folder->list(); __STP(filesList);
	filesList->setDeleteOnExit();

	int maxLoop = filesList->size();
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* path = filesList->get(i);

		File* f = folder->get(path); __STP(f);
		if(f->isDirectory()){
			scanFiles(f);
		} else{
			addCompilantUnit(f, folder);
		}
	}
}

void AbstractExecutableModuleInstance::addCompilantUnit(File *file, File *base) {
	int length = file->length();
	FileInputStream stream(file);

	this->contract->addCompilationUnit(&stream, length, base, file);
}

bool AbstractExecutableModuleInstance::hasCompileError() const noexcept {
	return !this->compile_errors->isEmpty();
}

bool AbstractExecutableModuleInstance::analyze() {
	this->vm->analyze();
	return !this->vm->hasError();
}

void AbstractExecutableModuleInstance::setMainInstance() {
	const UnicodeString* mainPackage = this->instanceConfig->getMainPackage();
	const UnicodeString* mainClass = this->instanceConfig->getMainClass();
	const UnicodeString* initializerMethod = this->instanceConfig->getInitializerMethod();

	this->contract->setMainMethod(mainPackage, mainClass, initializerMethod);
}

bool AbstractExecutableModuleInstance::createMainInstance() {
	this->mainInst = this->vm->createScInstance();
	ArrayList<Exception>& exceptions = this->vm->getExceptions();

	return !this->vm->hasError() && exceptions.isEmpty();
}

bool AbstractExecutableModuleInstance::interpretInitializer() {
	ArrayList<AbstractFunctionExtArguments> arguments;
	arguments.setDeleteOnExit();
	{
		const ArrayList<AbstractJsonValue>* argList = this->instanceConfig->getInitializerMethodArguments();
		int maxLoop = argList->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractJsonValue* jval = argList->get(i);

			AbstractFunctionExtArguments* arg = jval->toFunctionExtArgument();
			arguments.addElement(arg);
		}
	}

	const UnicodeString* initializerMethod = this->instanceConfig->getInitializerMethod();

	this->vm->interpretMainObjectMethod(initializerMethod, &arguments);

	ArrayList<Exception>& exceptions = this->vm->getExceptions();
	return !this->vm->hasError() && exceptions.isEmpty();
}

void AbstractExecutableModuleInstance::resetRootReference() {
	this->mainInst = nullptr;

	this->contract->clearRootReference(this->vm);

	GcManager* gc = this->vm->getGc();
	gc->garbageCollect();

	assert(gc->isEmpty());
}

} /* namespace alinous */
