/*
 * CdbStorageManager.h
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#ifndef TABLE_STORE_CDBSTORAGEMANAGER_H_
#define TABLE_STORE_CDBSTORAGEMANAGER_H_

#include "schema_table/schema/ISchemaUptateListner.h"

#include "base/HashMap.h"

namespace alinous {
class DiskCacheManager;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class SchemaManager;
class TableStore;
class CdbOid;
class CdbKeyFactory;
class CdbDataFactory;
class Schema;
class CdbTable;

class CdbStorageManager : public ISchemaUptateListner {
public:
	CdbStorageManager();

	virtual ~CdbStorageManager();

	void close();

	virtual void schemaLoaded(SchemaManager* sc);
	virtual void onCreateTable(SchemaManager* mgr, const CdbTable* table);
	virtual void onDropTable(SchemaManager* mgr, const CdbTable* table);

	virtual void onAlterModify(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const ColumnModifyContext* ctx);
	virtual void onDropPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey);
	virtual void onAddPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey);
	virtual void onAddColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableColumn* newColumn, const CdbTableIndex* newUniqueIndex);
	virtual void onDropColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableColumn* removalColumn, const ArrayList<CdbTableIndex>* removalIndexes);
	virtual void onAddIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* newIndex);
	virtual void onDropIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* removalIndex);
	virtual void onAlterTableRenameTable(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, TableRenameContext* ctx);

	TableStore* getTableStore(const CdbOid* tableoid) noexcept;

	void __testCheckIndexes();

private:
	void loadSchemaStore(SchemaManager* scMgr, const Schema* schema);
	void loadTableStore(SchemaManager* scMgr, const CdbTable* table);

	void handleUniqueKeyOnAlterModify(TableStore* store, const ColumnModifyContext* ctx);

public:
	static CdbKeyFactory keyFactory;
	static CdbDataFactory dataFactory;

private:
	SchemaManager* schemaManager;
	DiskCacheManager* cacheManager;

	HashMap<CdbOid, TableStore>* tableStoreMap;
};

} /* namespace codablecash */

#endif /* TABLE_STORE_CDBSTORAGEMANAGER_H_ */
