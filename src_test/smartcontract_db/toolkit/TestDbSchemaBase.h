/*
 * TestDbSchemaBase.h
 *
 *  Created on: 2020/08/24
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMABASE_H_
#define SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMABASE_H_
#include <cstdint>

#include "base/ArrayList.h"

#include "lang/sc_statement/StatementBlock.h"
namespace alinous {
class TestEnv;
class File;
class VirtualMachine;
class ExtExceptionObject;
class StackPopper;
class AnalyzeContext;
class SelectStatement;
class UpdateStatement;
class DeleteStatement;
class InsertStatement;
class StackVariableAccess;
class AbstractExtObject;
}
using namespace alinous;


namespace codablecash {

class CodableDatabase;
class CdbTableColumn;
class CdbTableIndex;
class CdbTable;
class IndexStore;
class CdbRecord;
class SchemaManager;
class CdbStorageManager;
class TableScanTarget;

class TestDbSchemaBase {
public:
	explicit TestDbSchemaBase(TestEnv* env);
	virtual ~TestDbSchemaBase();

	virtual void init();
	virtual void init(uint64_t memCapacity);

	void reloadDb();

	CodableDatabase* getDatabase() const noexcept;
	VirtualMachine* getVm() const noexcept {
		return this->vm;
	}

	void snapshot(const File* snapshotFile);

	const ExtExceptionObject* checkUncaughtException();

	CdbTable* getTable(const wchar_t* schema, const wchar_t* table);

	CdbTableColumn* getColumn(const wchar_t* schema, const wchar_t* table, const wchar_t* column);
	CdbTableColumn* getColumn(const wchar_t* table, const wchar_t* column);

	CdbTableIndex* getIndex(const wchar_t* schema, const wchar_t* table, const wchar_t* column);
	CdbTableIndex* getIndex(const wchar_t* table, const wchar_t* column);

	IndexStore* getIndexStore(const wchar_t* schema, const wchar_t* table, const wchar_t* column);
	IndexStore* getIndexStore(const wchar_t* table, const wchar_t* column);

	ArrayList<CdbRecord>* scanRecords(const wchar_t* table);
	ArrayList<CdbRecord>* scanRecords(const wchar_t* schema, const wchar_t* table);

	CdbTableIndex* getPrimaryKey(const wchar_t* table);
	CdbTableIndex* getPrimaryKey(const wchar_t* schema, const wchar_t* table);

	SchemaManager* getSchemaManager() const noexcept;
	CdbStorageManager* getStorageManager() const noexcept;

	bool execDDL(const File* sourceFile);
	void execSelectStmt(SelectStatement* stmt);
	void execUpdateStmt(UpdateStatement* stmt);
	void execDeleteStmt(DeleteStatement* stmt);
	void execInsertStmt(InsertStatement* stmt);

	void selectBlock(StatementBlock* block);

	void begin();
	void commit();
	void rollback();

	void setupTopStack();

	TableScanTarget* getScanTarget(const wchar_t* schema, const wchar_t* table) const;

	AbstractExtObject* getExtResultObj() const noexcept {
		return extResultObj;
	}

	const ExtExceptionObject* getExeptionExObj() const noexcept {
		return this->exobj;
	}

protected:
	void initSmartcontract();
	void setMain(const wchar_t* pkg, const wchar_t* clazz, const wchar_t* method) noexcept;
	void createDb();

protected:
	VirtualMachine* vm;
	TestEnv* env;

	uint64_t loidSerial;

	File* dbDir;
	File* undodbDir;

	File* folder; // smart contract base

	ExtExceptionObject* exobj;

	StackPopper* stackPopper;

	// returned value
	StackVariableAccess* access;
	AbstractExtObject* extResultObj;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMABASE_H_ */
