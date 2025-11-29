/*
 * AbstractExecutableModule.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */
#include "smartcontract_instance/AbstractExecutableModuleInstance.h"
#include "smartcontract_instance/InstanceDependencyHandler.h"
#include "smartcontract_instance/ExecutableModuleInstance.h"
#include "smartcontract_instance/LibraryExectableModuleInstance.h"
#include "smartcontract_instance/ModuleInstanceClassLoader.h"

#include "modular_project/ModularInstanceConfig.h"
#include "modular_project/DependencyConfig.h"
#include "modular_project/ModularConfigException.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"
#include "base/ArrayList.h"

#include "base_io/ByteBuffer.h"
#include "base_io/File.h"

#include "base_io_stream/exceptions.h"
#include "base_io_stream/FileInputStream.h"

#include "bc/ExceptionThrower.h"
#include "bc/SoftwareVersion.h"

#include "engine/CodableDatabase.h"

#include "engine/sc/SmartContract.h"
#include "engine/sc/CompilationUnit.h"

#include "engine/compiler/CompileError.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "vm/VirtualMachine.h"

#include "json_object/AbstractJsonValue.h"

#include "ext_arguments/AbstractFunctionExtArguments.h"

#include "instance/instance_gc/GcManager.h"

#include "lang/sc_declare/ClassDeclare.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

AbstractExecutableModuleInstance::AbstractExecutableModuleInstance(uint8_t kind) {
	this->kind = kind;
	this->name = nullptr;
	this->projectRelativePath = nullptr;

	this->sourceFolders = new ArrayList<UnicodeString>();
	this->version = nullptr;
	this->instanceConfig = nullptr;
	this->dependencyConfig = nullptr;

	this->vm = nullptr;
	this->compile_errors = nullptr;
	this->mainInst = nullptr;
	this->dependencyHandler = new InstanceDependencyHandler();

	this->dbDir = nullptr;
	this->undodbDir = nullptr;
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

	this->mainInst = nullptr;
	delete this->dependencyHandler;

	delete this->dbDir;
	delete this->undodbDir;
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
	delete this->vm;
	this->vm = new VirtualMachine(1024*1024);

	SmartContract* contract = new SmartContract();
	this->vm->loadSmartContract(contract);

	this->compile_errors = nullptr;
}

void AbstractExecutableModuleInstance::parseSourceFolders(const File *projectBaseDir) {
	File* baseDir = projectBaseDir->get(this->projectRelativePath); __STP(baseDir);

	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->exists(), L"Module base folder does not exists.", __FILE__, __LINE__);
	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->isDirectory(), L"Module base folder id not a directory.", __FILE__, __LINE__);

	int maxLoop = this->sourceFolders->size();
	for(int i = 0; i != maxLoop; ++i){
		const UnicodeString* folder = this->sourceFolders->get(i);

		scanSourceFolder(baseDir, folder, projectBaseDir);
	}
}

void AbstractExecutableModuleInstance::scanSourceFolder(File *baseDir, const UnicodeString *folder, const File *projectBaseDir) {
	File* dir = baseDir->get(folder); __STP(dir);
	scanFiles(dir, projectBaseDir);

	SmartContract* contract = this->vm->getSmartContract();
	this->compile_errors = contract->getCompileErrors();
}

void AbstractExecutableModuleInstance::scanFiles(File *folder, const File *projectBaseDir) {
	ArrayList<UnicodeString>* filesList = folder->list(); __STP(filesList);
	filesList->setDeleteOnExit();

	int maxLoop = filesList->size();
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* path = filesList->get(i);

		File* f = folder->get(path); __STP(f);
		if(f->isDirectory()){
			scanFiles(f, projectBaseDir);
		} else{
			addCompilantUnit(f, folder, projectBaseDir);
		}
	}
}

void AbstractExecutableModuleInstance::addCompilantUnit(File *file, File *base, const File *projectBaseDir) {
	int length = file->length();
	FileInputStream stream(file);

	SmartContract* contract = this->vm->getSmartContract();
	CompilationUnit* unit = contract->addCompilationUnit(&stream, length, base, file);

	const UnicodeString* projectPath = projectBaseDir->getAbsolutePath(); __STP(projectPath);
	const UnicodeString* filePath = file->getAbsolutePath(); __STP(filePath);

	UnicodeString* path = filePath->substring(projectPath->length()); __STP(path);

	unit->setProjectRelativePath(path);
}

bool AbstractExecutableModuleInstance::hasCompileError() const noexcept {
	return !this->compile_errors->isEmpty();
}


void AbstractExecutableModuleInstance::setMainInstance() {
	const UnicodeString* mainPackage = this->instanceConfig->getMainPackage();
	const UnicodeString* mainClass = this->instanceConfig->getMainClass();
	const UnicodeString* initializerMethod = this->instanceConfig->getInitializerMethod();

	SmartContract* contract = this->vm->getSmartContract();
	contract->setMainMethod(mainPackage, mainClass, initializerMethod);
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
	SmartContract* contract = this->vm->getSmartContract();

	if(contract->isInitialized()){
		this->mainInst = nullptr;

		contract->clearRootReference(this->vm);

		GcManager* gc = this->vm->getGc();
		gc->garbageCollect();

		assert(gc->isEmpty());
	}
}

bool AbstractExecutableModuleInstance::initBeforeAnalyze() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->initBeforeAnalyze(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}

bool AbstractExecutableModuleInstance::preAnalyze() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->preAnalyze(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}

bool AbstractExecutableModuleInstance::preAnalyzeGenerics() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->preAnalyzeGenerics(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}

bool AbstractExecutableModuleInstance::analyzeType() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->analyzeType(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}


bool AbstractExecutableModuleInstance::analyzeMetadata() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->analyzeMetadata(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}

bool AbstractExecutableModuleInstance::analyzeFinal() {
	SmartContract* contract = this->vm->getSmartContract();
	contract->analyzeFinal(this->vm);

	AnalyzeContext* actx = contract->getAnalyzeContext();
	return actx->hasError();
}

bool AbstractExecutableModuleInstance::loadDependency(ModularSmartcontractInstance* parent) {
	SmartContract* contract = this->vm->getSmartContract();
	AnalyzeContext* actx = contract->getAnalyzeContext();

	if(this->dependencyConfig != nullptr){
		ArrayList<AbstractDependencyConfig>* list = this->dependencyConfig->getList();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractDependencyConfig* config = list->get(i);

			this->dependencyHandler->registerDependentInstance(config, parent, actx);
		}
	}

	this->dependencyHandler->importExportedClasses(actx);

	return actx->hasError();
}

bool AbstractExecutableModuleInstance::preAnalyzeDependency() {
	SmartContract* contract = this->vm->getSmartContract();
	AnalyzeContext* actx = contract->getAnalyzeContext();

	this->dependencyHandler->preAnalyze(actx);

	return actx->hasError();
}

bool AbstractExecutableModuleInstance::analyzeTypeDependency() {
	SmartContract* contract = this->vm->getSmartContract();
	AnalyzeContext* actx = contract->getAnalyzeContext();

	this->dependencyHandler->analyzeType(actx);

	return actx->hasError();
}

bool AbstractExecutableModuleInstance::analyzeDependency() {
	SmartContract* contract = this->vm->getSmartContract();
	AnalyzeContext* actx = contract->getAnalyzeContext();

	this->dependencyHandler->analyze(actx);

	return actx->hasError();
}

void AbstractExecutableModuleInstance::loadLibExportInterfaceUnits(ModuleInstanceClassLoader* clazzloader) {
	SmartContract* contract = this->vm->getSmartContract();

	ArrayList<UnicodeString>* liblist = this->instanceConfig->getLibExport();
	int maxLoop = liblist->size();
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* fqn = liblist->get(i);

		// load class
		clazzloader->loadClass(fqn);

		const ClassDeclare* dec = clazzloader->getClassDeclare(fqn);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(dec == nullptr, L"Module does not exists.", __FILE__, __LINE__);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(!dec->isInterface(), L"libExport must be an interface, not a class.", __FILE__, __LINE__);
	}
}

int AbstractExecutableModuleInstance::binarySize() const {
	BinaryUtils::checkNotNull(this->name);
	BinaryUtils::checkNotNull(this->projectRelativePath);
	BinaryUtils::checkNotNull(this->version);
	BinaryUtils::checkNotNull(this->instanceConfig);


	int total = sizeof(uint8_t);
	total += BinaryUtils::stringSize(this->name);
	total += BinaryUtils::stringSize(this->projectRelativePath);

	int maxLoop = this->sourceFolders->size();
	total += sizeof(uint16_t);
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* folder = this->sourceFolders->get(i);
		total += BinaryUtils::stringSize(folder);
	}

	total += this->version->binarySize();
	total += this->instanceConfig->binarySize();

	total += sizeof(uint8_t);
	if(this->dependencyConfig != nullptr){
		total += this->dependencyConfig->binarySize();
	}

	// vm to binary
	SmartContract* contract = this->vm->getSmartContract();

	maxLoop = contract->getNumCompilationUnit();
	total += sizeof(uint32_t);

	for(int i = 0; i != maxLoop; ++i){
		CompilationUnit* unit = contract->getCompilationUnit(i);
		total += unit->binarySize();
	}

	return total;
}

void AbstractExecutableModuleInstance::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->name);
	BinaryUtils::checkNotNull(this->projectRelativePath);
	BinaryUtils::checkNotNull(this->version);
	BinaryUtils::checkNotNull(this->instanceConfig);

	out->put(this->kind);
	BinaryUtils::putString(out, this->name);
	BinaryUtils::putString(out, this->projectRelativePath);

	int maxLoop = this->sourceFolders->size();
	out->putShort(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* folder = this->sourceFolders->get(i);
		BinaryUtils::putString(out, folder);
	}

	this->version->toBinary(out);
	this->instanceConfig->toBinary(out);

	uint8_t bl = (this->dependencyConfig != nullptr) ? 1 : 0;
	out->put(bl);
	if(bl > 0){
		this->dependencyConfig->toBinary(out);
	}

	// vm to binary
	SmartContract* contract = this->vm->getSmartContract();

	maxLoop = contract->getNumCompilationUnit();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CompilationUnit* unit = contract->getCompilationUnit(i);
		unit->toBinary(out);
	}

}

void AbstractExecutableModuleInstance::fromBinary(ByteBuffer *in) {
	this->name = BinaryUtils::getString(in);
	this->projectRelativePath = BinaryUtils::getString(in);

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* folder = BinaryUtils::getString(in);
		this->sourceFolders->addElement(folder);
	}

	this->version = SoftwareVersion::createFromBinary(in);
	this->instanceConfig = ModularInstanceConfig::createFromBinary(in);

	uint8_t bl = in->get();
	if(bl > 0){
		this->dependencyConfig = DependencyConfig::createFromBinary(in);
	}

	// vm
	resetContract();
	SmartContract* contract = this->vm->getSmartContract();

	maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		CodeElement* element = CodeElement::createFromBinary(in); __STP(element);
		CompilationUnit* unit = dynamic_cast<CompilationUnit*>(element);
		BinaryUtils::checkNotNull(unit);

		__STP_MV(element);
		contract->addCompilationUnit(unit);
	}
}

AbstractExecutableModuleInstance* AbstractExecutableModuleInstance::createFromBinary(ByteBuffer *in) {
	AbstractExecutableModuleInstance* inst = nullptr;

	uint8_t kind = in->get();
	if(kind == TYPE_EXEC){
		inst = new ExecutableModuleInstance();
	}
	else{
		inst = new LibraryExectableModuleInstance();
	}
	__STP(inst);

	inst->fromBinary(in);

	return __STP_MV(inst);
}

void AbstractExecutableModuleInstance::setDatabaseDir(const File *baseDir) {
	File* dbRoot = baseDir->get(this->name); __STP(dbRoot);

	this->dbDir = dbRoot->get(DB_DIR);
	this->undodbDir = dbRoot->get(UNDO_DIR);
}

void AbstractExecutableModuleInstance::createDatabase() {
	CodableDatabase db;
	db.createDatabase(this->dbDir, this->undodbDir);
}

void AbstractExecutableModuleInstance::loadDatabase() {
	this->vm->loadDatabase(this->dbDir, this->undodbDir);
}

} /* namespace alinous */
