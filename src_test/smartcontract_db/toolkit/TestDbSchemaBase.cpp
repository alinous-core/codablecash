/*
 * TestDbSchemaBase.cpp
 *
 *  Created on: 2020/08/24
 *      Author: iizuka
 */

#include "TestDbSchemaBase.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbException.h"

#include "../../test_utils/TestEnv.h"
#include "vm/VirtualMachine.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "base_io_stream/FileInputStream.h"

#include "ext_binary/ExtExceptionObject.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "scan_select/scan/RecordScanner.h"

#include "scan_select/scan_table/TableScanTarget.h"

#include "engine/compiler/SmartContractParser.h"

#include "engine/sc/SmartContract.h"

#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql/AbstractSQLStatement.h"

#include "engine/sc_analyze/AnalyzeContext.h"

#include "vm/stack/StackPopper.h"

#include "engine/sc_analyze_stack/AnalyzeStackPopper.h"

#include "engine/sc_analyze_stack/AnalyzeStackManager.h"

#include "lang_sql/sql_dml/SelectStatement.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "vm/VmSelectPlannerSetter.h"

#include "vm/variable_access/StackVariableAccess.h"

#include "instance/AbstractVmInstance.h"

#include "vm/vm_trx/VmTransactionHandler.h"

#include "snapshot/FileStoreSnapshot.h"
namespace codablecash {

TestDbSchemaBase::TestDbSchemaBase(TestEnv* env) {
	this->env = env;
	this->dbDir = nullptr;
	this->undodbDir = nullptr;
	this->vm = nullptr;
	this->loidSerial = 1;

	this->folder = nullptr;
	this->exobj = nullptr;

	this->stackPopper = nullptr;
	this->access = nullptr;
	this->extResultObj = nullptr;
}

TestDbSchemaBase::~TestDbSchemaBase() {
	delete this->stackPopper;

	this->env = nullptr;
	delete this->dbDir;
	delete this->undodbDir;
	delete this->vm;
	delete this->folder;
	delete this->exobj;
	delete this->access;
	delete this->extResultObj;
}

void TestDbSchemaBase::init() {
	init(1024 * 10);
}

void TestDbSchemaBase::init(uint64_t memCapacity) {
	this->vm = new VirtualMachine(memCapacity);

	initSmartcontract();
	createDb();

	this->vm->loadDatabase(this->dbDir, this->undodbDir);
}

void TestDbSchemaBase::snapshot(const File *snapshotFile) {
	CodableDatabase* db = getDatabase();

	FileStoreSnapshot handler;
	handler.openOutFile(snapshotFile);
	db->snapshot(&handler);

	handler.close();
}

void TestDbSchemaBase::reloadDb() {
	CodableDatabase* db = getDatabase();

	db->closeDatabase();
	db->loadDatabase(this->dbDir, this->undodbDir);
}

CodableDatabase* TestDbSchemaBase::getDatabase() const noexcept {
	return this->vm->getDb();
}

void TestDbSchemaBase::initSmartcontract() {
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/toolkit/resources/sc/SmartContract.alns"))

	this->folder = projectFolder->get(L"src_test/smartcontract_db/toolkit/resources/sc/");

	SmartContract* sc = new SmartContract();

	int length = sourceFile->length();
	FileInputStream stream(sourceFile);

	sc->addCompilationUnit(&stream, length, this->folder, sourceFile);

	this->vm->loadSmartContract(sc);

	this->vm->analyze();

	setMain(L"test.fw", L"SmartContract", L"main");
	this->vm->createScInstance();
}

void TestDbSchemaBase::setMain(const wchar_t* pkg, const wchar_t* clazz, const wchar_t* method) noexcept {
	UnicodeString mainPackage(pkg);
	UnicodeString mainClass(clazz);
	UnicodeString mainMethod(method);

	this->vm->getSmartContract()->setMainMethod(&mainPackage, &mainClass, &mainMethod);
}

const ExtExceptionObject* TestDbSchemaBase::checkUncaughtException() {
	this->vm->checkUncaughtException();

	this->exobj = this->vm->getUncaughtException();

	return this->exobj;
}

void TestDbSchemaBase::createDb() {
	File testCaseFolder = this->env->testCaseDir();
	this->dbDir = testCaseFolder.get(L"db");
	this->undodbDir = testCaseFolder.get(L"dbundo");
	{
		CodableDatabase db;
		db.createDatabase(this->dbDir, this->undodbDir);
	}
}

ArrayList<CdbRecord>* TestDbSchemaBase::scanRecords(const wchar_t* table) {
	return scanRecords(SchemaManager::PUBLIC.towString(), table);
}

ArrayList<CdbRecord>* TestDbSchemaBase::scanRecords(const wchar_t* schema, const wchar_t* table) {
	CodableDatabase* db = getDatabase();
	CdbStorageManager* storageMgr = db->getStorageManager();

	CdbTable* cdbtable = getTable(schema, table);

	TableStore* store = storageMgr->getTableStore(cdbtable->getOid());

	RecordScanner scanner(store);
	scanner.start();

	ArrayList<CdbRecord>* list = new ArrayList<CdbRecord>();
	while(scanner.hasNext()){
		const CdbRecord* record = scanner.next();

		list->addElement(new CdbRecord(*record));
	}

	return list;
}

CdbTableColumn* TestDbSchemaBase::getColumn(const wchar_t* schema, const wchar_t* table, const wchar_t* column) {
	CdbTable* cdbtable = getTable(schema, table);

	CdbTableColumn* col = cdbtable->getColumn(column);

	return col;
}

CdbTableColumn* TestDbSchemaBase::getColumn(const wchar_t* table, const wchar_t* column) {
	const wchar_t* s = SchemaManager::PUBLIC.towString();

	return getColumn(s, table, column);
}


CdbTable* TestDbSchemaBase::getTable(const wchar_t* schema, const wchar_t* table) {
	CodableDatabase* db = getDatabase();
	SchemaManager* scMgr = db->getSchemaManager();

	UnicodeString schemastr(schema);
	UnicodeString tableName(table);
	CdbTable* cdbtable = scMgr->getTable(&schemastr, &tableName);

	return cdbtable;
}

CdbTableIndex* TestDbSchemaBase::getIndex(const wchar_t* schema, const wchar_t* table, const wchar_t* column) {
	CodableDatabase* db = getDatabase();
	CdbStorageManager* storageMgr = db->getStorageManager();

	CdbTable* cdbtable = getTable(schema, table);

	CdbTableColumn* col = getColumn(schema, table, column);
	CdbTableIndex* index = cdbtable->getIndexByColumnOid(col->getOid());

	//TableStore* store = storageMgr->getTableStore(cdbtable->getOid());
	//CdbTableIndex* indexStore = store->getIndexStore(index->getOid());

	return index;
}

CdbTableIndex* TestDbSchemaBase::getIndex(const wchar_t* table, const wchar_t* column) {
	const wchar_t* s = SchemaManager::PUBLIC.towString();

	return getIndex(s, table, column);
}

IndexStore* TestDbSchemaBase::getIndexStore(const wchar_t* table, const wchar_t* column) {
	const wchar_t* s = SchemaManager::PUBLIC.towString();

	return getIndexStore(s, table, column);
}

IndexStore* TestDbSchemaBase::getIndexStore(const wchar_t* schema, const wchar_t* table, const wchar_t* column) {
	CodableDatabase* db = getDatabase();
	CdbStorageManager* storageMgr = db->getStorageManager();

	CdbTable* cdbtable = getTable(schema, table);

	TableStore* store = storageMgr->getTableStore(cdbtable->getOid());

	CdbTableIndex* index = getIndex(schema, table, column);
	if(index == nullptr){
		return nullptr;
	}

	IndexStore* indexStore = store->getIndexStore(index->getOid());

	return indexStore;
}

CdbTableIndex* TestDbSchemaBase::getPrimaryKey(const wchar_t* table) {
	const wchar_t* s = SchemaManager::PUBLIC.towString();

	return getPrimaryKey(s, table);
}

CdbTableIndex* TestDbSchemaBase::getPrimaryKey(const wchar_t* schema, const wchar_t* table) {
	CdbTable* cdbtable = getTable(schema, table);

	return cdbtable->getPrimaryKey();
}

SchemaManager* TestDbSchemaBase::getSchemaManager() const noexcept {
	return this->vm->getDb()->getSchemaManager();
}

CdbStorageManager* TestDbSchemaBase::getStorageManager() const noexcept {
	return this->vm->getDb()->getStorageManager();
}

bool TestDbSchemaBase::execDDL(const File* sourceFile) {
	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLStatement* stmt = lang->ddlStatement(); __STP(stmt);
	if(stmt == nullptr){
		throw new CdbException(L"failed in parsing.", __FILE__, __LINE__);
	}

	AnalyzeContext* actx = this->vm->getSmartContract()->getAnalyzeContext(); //new AnalyzeContext(nullptr); __STP(actx);
	//actx->setVm(this->vm);

	stmt->preAnalyze(actx);
	stmt->analyzeTypeRef(actx);
	stmt->analyze(actx);

	if(this->vm->hasError()){
		return false;
	}

	stmt->interpret(this->vm);

	return true;
}


void TestDbSchemaBase::selectBlock(StatementBlock *block) {
	CompilationUnit unit;
	block->setParent(&unit);

	AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
	actx->setVm(this->vm);

	ArrayList<AbstractStatement>* list = block->getStatements();

	const UnicodeString* intoVar = nullptr;
	{
		AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
		AnalyzeStackPopper popper(stackMgr, true);
		stackMgr->addFunctionStack();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractStatement* stmt = list->get(i);

			short kind = stmt->getKind();
			if(kind == CodeElement::DML_STMT_SELECT){
				SelectStatement* selectStmt = dynamic_cast<SelectStatement*>(stmt);

				selectStmt->preAnalyze(actx);
				selectStmt->analyzeTypeRef(actx);
				selectStmt->analyze(actx);

				intoVar = selectStmt->getIntoVar();
				this->access = stackMgr->findStackVariableAccess(intoVar);
			}
			else{
				stmt->preAnalyze(actx);
				stmt->analyzeTypeRef(actx);
				stmt->analyze(actx);
			}
		}
	}

	// interpret
	{
		setupTopStack();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractStatement* stmt = list->get(i);

			stmt->init(this->vm);
		}

		for(int i = 0; i != maxLoop; ++i){
			AbstractStatement* stmt = list->get(i);

			stmt->interpret(vm);
		}

		checkUncaughtException();

		if(this->exobj != nullptr){
			return;
		}

		AbstractVmInstance* inst = this->access->interpret(this->vm, nullptr);
		this->extResultObj = inst->toClassExtObject(intoVar, nullptr);
	}

}

void TestDbSchemaBase::execSelectStmt(SelectStatement* stmt) {
	AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
	actx->setVm(this->vm);

	const UnicodeString* intoVar = nullptr;
	{
		AnalyzeStackManager* stackMgr = actx->getAnalyzeStackManager();
		AnalyzeStackPopper popper(stackMgr, true);
		stackMgr->addFunctionStack();

		stmt->preAnalyze(actx);
		stmt->analyzeTypeRef(actx);
		stmt->analyze(actx);

		intoVar = stmt->getIntoVar();
		this->access = stackMgr->findStackVariableAccess(intoVar);
	}

	SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
	VmSelectPlannerSetter setter(vm, planner);

	setupTopStack();

	stmt->init(vm);
	stmt->interpret(vm);

	checkUncaughtException();

	if(this->exobj != nullptr){
		return;
	}

	AbstractVmInstance* inst = this->access->interpret(this->vm, nullptr);
	this->extResultObj = inst->toClassExtObject(intoVar, nullptr);
}

void TestDbSchemaBase::execUpdateStmt(UpdateStatement* stmt) {
	AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
	actx->setVm(this->vm);

	stmt->preAnalyze(actx);
	stmt->analyzeTypeRef(actx);
	stmt->analyze(actx);

	stmt->init(this->vm);
	stmt->interpret(this->vm);
}

void TestDbSchemaBase::execDeleteStmt(DeleteStatement* stmt){
	AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
	actx->setVm(this->vm);

	stmt->preAnalyze(actx);
	stmt->analyzeTypeRef(actx);
	stmt->analyze(actx);

	stmt->init(this->vm);
	stmt->interpret(this->vm);
}

void TestDbSchemaBase::execInsertStmt(InsertStatement* stmt) {
	AnalyzeContext* actx = new AnalyzeContext(nullptr); __STP(actx);
	actx->setVm(this->vm);

	stmt->preAnalyze(actx);
	stmt->analyzeTypeRef(actx);
	stmt->analyze(actx);

	stmt->init(this->vm);
	stmt->interpret(this->vm);
}

void TestDbSchemaBase::begin() {
	VmTransactionHandler* trxHandler = this->vm->getTransactionHandler();
	trxHandler->begin();
}

void TestDbSchemaBase::commit() {
	VmTransactionHandler* trxHandler = this->vm->getTransactionHandler();
	trxHandler->commit();
}

void TestDbSchemaBase::rollback() {
	VmTransactionHandler* trxHandler = this->vm->getTransactionHandler();
	trxHandler->rollback();
}


void TestDbSchemaBase::setupTopStack() {
	GcManager* gc = vm->getGc();
	vm->newStack();

	this->stackPopper = new StackPopper(this->vm);
}

TableScanTarget* TestDbSchemaBase::getScanTarget(const wchar_t* schema, const wchar_t* table) const {
	SelectScanPlanner* planner = this->vm->getSelectPlanner();

	TableScanTarget* target = new TableScanTarget(schema, table);
	target->resolveTable(this->vm, planner);

	return target;
}

} /* namespace codablecash */
