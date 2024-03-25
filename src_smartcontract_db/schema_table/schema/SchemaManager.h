/*
 * Schema.h
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#ifndef SCHEMA_SCHEMAMANAGER_H_
#define SCHEMA_SCHEMAMANAGER_H_
#include <cstdint>

#include "base/ArrayList.h"
#include "base/UnicodeString.h"

namespace alinous {
class UnicodeString;
class File;
class TableIdentifier;

}  // namespace alinous
using namespace alinous;

namespace codablecash {

class SchemaRoot;
class Schema;
class ISchemaUptateListner;
class CdbTable;
class AbstractAlterCommandLog;

class AlterAddColumnCommandLog;
class AlterAddIndexCommandLog;
class AlterDropIndexCommandLog;
class AlterAddPrimaryKeyCommandLog;
class AlterDropPrimaryKeyCommandLog;
class AlterDropColumnCommandLog;
class AlterModifyCommandLog;
class AlterRenameColumnCommandLog;
class AlterRenameTableCommandLog;
class ColumnModifyContext;
class CdbTableIndex;
class SchemaAlterCommandsHandler;
class CodableDatabase;
class TableRenameContext;
class CdbTableColumn;
class ISnapshotHandler;
class CdbStorageManager;
class SchemaCompare;
class CdbOid;
class CdbTransactionManager;

class SchemaManager {
public:
	friend class SchemaAlterCommandsHandler;
	friend class UndoAlterAddIndexLog;

	static const UnicodeString PUBLIC;
	static const UnicodeString SCHEMA_FILE;

	explicit SchemaManager(CodableDatabase* db);
	virtual ~SchemaManager();

	void addSchemaUpdateListner(ISchemaUptateListner* listner) noexcept;

	static void createBlankSchema(File* baseDir);
	static void createSchema(const UnicodeString* name, File* baseDir);

	void createSchema(const UnicodeString* name);
	void recoverSchema(Schema* schema);

	void loadSchema(const File* baseDir);
	void save();

	uint64_t newTransactionId();
	uint64_t newSchemaObjectId() noexcept;
	uint64_t newRecordObjectId() noexcept;
	uint64_t newCommitId() noexcept;

	void snapshotObjectIds(ISnapshotHandler *handler) noexcept;
	void snapshot(ISnapshotHandler *handler, CdbStorageManager* storeManager);
	void recoverObjectIds(uint64_t maxTransactionId, uint64_t maxObjectId, uint64_t maxSchemaObjectId, uint64_t maxCommitId);

	void __testCheckEqualsObjectIds(SchemaManager* other);
	void __testCheckEquals(CdbStorageManager* storeManager, SchemaManager* other, CdbStorageManager* otherStoreManager);

	uint64_t getSchemaObjectVersionId() const noexcept;

	void createTable(CdbTable* table);
	void dropTable(const TableIdentifier* tableId);
	void removeTable(const UnicodeString* schema, const UnicodeString* name);

	void recoverTable(CdbTable* table);
	void recoverIndex(CdbTransactionManager* trxManager, CdbTableIndex* index, const UnicodeString* schemaName, const UnicodeString* tableName);

	Schema* getSchema(const UnicodeString* name) const noexcept;
	const ArrayList<Schema, SchemaCompare>* getSchemaList() const noexcept;
	void removeSchema(const UnicodeString* name) noexcept;

	CdbTable* getTable(const wchar_t* schema, const wchar_t* name) const;
	CdbTable* getTable(const UnicodeString* schema, const UnicodeString* name) const;
	CdbTable* getTable(const TableIdentifier* tableId, const UnicodeString* defaultSchema) const;
	CdbTable* getTable(const CdbOid* tableOid) const;

	bool hasTable(const TableIdentifier* tableId) const noexcept;
	bool hasTable(const UnicodeString* schema, const UnicodeString* name) const noexcept;

	const File* getDatabaseBaseDir() const noexcept {
		return databaseBaseDir;
	}

	void handleAlterTableAddIndex(const AlterAddIndexCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableAddColumn(const AlterAddColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropIndex(const AlterDropIndexCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropColumn(const AlterDropColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableAddPrimaryKey(const AlterAddPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropPrimaryKey(const AlterDropPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableModify(const AlterModifyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableRenameColumn(const AlterRenameColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableRenameTable(const AlterRenameTableCommandLog* cmd, CdbTransactionManager* trxManager);

private:
	void fireSchemaLoaded() noexcept;
	void fireOnCreateTable(const CdbTable* table);
	void fireOnDropTable(const CdbTable* table);
	void fireOnAlterModify(CdbTransactionManager* trxManager, const CdbTable* table, const ColumnModifyContext* ctx);
	void fireOnDropPrimaryKey(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableIndex* primaryKey);
	void fireOnAddPrimaryKey(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableIndex* primaryKey);
	void fireOnAddColumn(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableColumn* newColumn, const CdbTableIndex* newUniqueIndex);
	void fireOnDropColumn(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableColumn* removalColumn, const ArrayList<CdbTableIndex>* removalIndexes);
	void fireOnAddIndex(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableIndex* newIndex);
	void fireOnDropIndex(CdbTransactionManager* trxManager, const CdbTable* table, const CdbTableIndex* removalIndex);
	void fireOnRenameTable(CdbTransactionManager* trxManager, const CdbTable* table, TableRenameContext* context);

	CdbTable* findTableFromCommand(const AbstractAlterCommandLog* cmdlog);

private:
	SchemaRoot* root;
	File* schemaBin;
	File* databaseBaseDir;

	SchemaAlterCommandsHandler* alterHandler;

	ArrayList<ISchemaUptateListner> listners;
};

} /* namespace alinous */

#endif /* SCHEMA_SCHEMAMANAGER_H_ */
