/*
 * TableStore.h
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#ifndef TABLE_STORE_TABLESTORE_H_
#define TABLE_STORE_TABLESTORE_H_

#include "base/HashMap.h"
#include "base/ArrayList.h"

namespace alinous {
class File;
class DiskCacheManager;
}
using namespace alinous;

namespace codablecash {

class CdbTable;
class RecordStore;
class CdbOid;
class IndexStore;
class CdbRecord;
class AbstractCdbValue;
class CdbTableColumn;
class CdbTableIndex;
class ColumnModifyContext;
class TableLevelLock;
class AbstractLockHandle;
class TableRenameContext;
class SchemaManager;
class CdbTransactionManager;
class CdbOidValueList;
class CdbDatabaseSession;

class TableStore {
public:
	TableStore(DiskCacheManager* cacheManager, const File* baseDir, const CdbTable* table);
	virtual ~TableStore();

	static void cleanTableStore(const CdbTable* table, const TableStore* store);

	const CdbOid* getOid() const noexcept;

	void createTable();
	void loadTable();
	void closeTable();
	const CdbTable* getTable() const noexcept;

	void insert(const CdbRecord* rec);
	void remove(const CdbOid* oid);
	void validateRecord(CdbRecord* rec);

	void addNewColumn(const CdbTableColumn* newColumn);
	void removeColumn(const CdbTableColumn* removalColumn);

	void modifyRecords(CdbTransactionManager* trxManager, const ColumnModifyContext* ctx);
	void buildIndex(const CdbTableIndex* index);
	void buildAllIndexes();

	RecordStore* getRecordStore() const noexcept {
		return this->recordStore;
	}

	void addNewIndex(const CdbTableIndex* index);
	void removeIndex(const CdbTableIndex* index);
	void resetAllIndexes();

	IndexStore* getIndexStore(const CdbOid* indexoid) const noexcept;

	CdbRecord* findRecord(const CdbOid* recordOid);

	AbstractLockHandle* writeLock();
	AbstractLockHandle* readLock();

	void onRename(SchemaManager* mgr, TableRenameContext* ctx);

	void __testCheckIndexes();
	void __testCheckRecordIndex(const CdbRecord* record);
	static void __checkNotNull(CdbOidValueList* oidlist);
	static void __checkIsUnique(CdbOidValueList* oidlist);

	void addUndoDeleteColumnPart(CdbDatabaseSession* session, int pos) const;

private:
	void validateRecordColumnValue(CdbTableColumn* meta, AbstractCdbValue* value);
	void addToIndexes(const CdbRecord* rec);
	void removeFromIndexes(const CdbRecord* rec, const CdbOid* oid);
	void addRecord2Index(const ArrayList<IndexStore>* indexStoreList, const CdbRecord* rec);

private:
	DiskCacheManager* cacheManager;

	File* baseDir;
	const CdbTable* table;

	RecordStore* recordStore;
	HashMap<CdbOid, IndexStore>* indexStores;

	TableLevelLock* tableLock;
};

} /* namespace codablecash */

#endif /* TABLE_STORE_TABLESTORE_H_ */
