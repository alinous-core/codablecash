/*
 * DataHistoryStore.h
 *
 *  Created on: 2023/09/13
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATAHISTORYSTORE_H_
#define DATA_HISTORY_DATAHISTORYSTORE_H_

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
}
using namespace alinous;

namespace codablecash {

class TransferedDataId;
class AbstractTransferedData;

class DataHistoryStore {
public:
	static const constexpr wchar_t* DIR_NAME{L"datahistory"};
	static const constexpr wchar_t* MAIN_DATA{L"main_data"};
	static const constexpr wchar_t* TIMEINDEX_DATA{L"timeindex_data"};

	explicit DataHistoryStore(const File* baseDir);
	virtual ~DataHistoryStore();

	void createBlankDatabase();
	void open();
	void close();

	void add(const TransferedDataId* dataId, const AbstractTransferedData* data);
	bool hasId(const TransferedDataId* dataId) const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;
	Btree* timeIndex;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATAHISTORYSTORE_H_ */
