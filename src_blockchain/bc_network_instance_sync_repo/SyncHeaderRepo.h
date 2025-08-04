/*
 * SyncHeaderRepo.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERREPO_H_
#define BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERREPO_H_

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class ULongKey;
}
using namespace alinous;

namespace codablecash {

class SyncHeaderHeightData;
class HeightHeaderSyncDataList;
class ClientSyncHeaderHeightData;

class SyncHeaderRepo {
public:
	static const constexpr wchar_t* DIR_NAME{L"headerdata"};
	static const constexpr wchar_t* MAIN_DATA{L"main_data"};

	explicit SyncHeaderRepo(const File* baseDir);
	virtual ~SyncHeaderRepo();

	void createBlankDatabase();
	void open();
	void close();

	void add(const SyncHeaderHeightData *headerData);
	void add(const ClientSyncHeaderHeightData *headerData);

	HeightHeaderSyncDataList* getFirst() const;
	ULongKey* getFirstKey() const;

	bool isEmpty() const;
	HeightHeaderSyncDataList* fetchFirst() const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERREPO_H_ */
