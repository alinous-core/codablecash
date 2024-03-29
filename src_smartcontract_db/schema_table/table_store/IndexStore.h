/*
 * IndexStore.h
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#ifndef TABLE_STORE_INDEXSTORE_H_
#define TABLE_STORE_INDEXSTORE_H_

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
}
using namespace alinous;

namespace codablecash {

class CdbTable;
class CdbTableIndex;
class CdbOid;
class CdbRecord;
class IndexScanner;
class AbstractCdbKey;
class CdbRecordKey;
class CdbOidValueList;

class IndexStore {
public:
	IndexStore(DiskCacheManager* cacheManager, const File* tableDir, const CdbTable* table, const CdbTableIndex* index);
	virtual ~IndexStore();

	static void createStore(const File* tableDir, const CdbTable* table, const CdbTableIndex* index, DiskCacheManager* cacheManager);
	static void cleanupStore(const File* tableDir, const CdbTable* table, const CdbTableIndex* index);

	void load();
	void close() noexcept;

	const CdbOid* getIndexOid() const noexcept;

	void insert(const CdbRecord* rec);
	void removeIndex(const CdbRecord* rec, const CdbOid* oid);
	bool hasKey(const CdbRecordKey* key, const CdbOid* ignoreOid) const noexcept;
	void reset();

	CdbOidValueList* find(const CdbRecordKey* key);

	IndexScanner* getScanner(const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq);

	Btree* getBtree() const noexcept;

	CdbRecordKey* makeKey(const CdbRecord* rec);

private:
	DiskCacheManager* cacheManager;

	File* tableDir;
	const CdbTable* table;
	const CdbTableIndex* index;

	Btree* btree;
	bool opened;
};

} /* namespace codablecash */

#endif /* TABLE_STORE_INDEXSTORE_H_ */
