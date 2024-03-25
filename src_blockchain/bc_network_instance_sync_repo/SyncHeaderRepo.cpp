/*
 * SyncHeaderRepo.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "bc_network_instance_sync_repo/SyncHeaderRepo.h"
#include "bc_network_instance_sync_repo/SyncHeaderDataFactory.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "bc_block/BlockHeader.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "bc_network_instance_sync_repo/HeightHeaderSyncDataList.h"

#include "data_history/TransferedDataId.h"

#include "bc_p2p_cmd_node/SyncHeaderHeightData.h"

#include "btree/BtreeScanner.h"
namespace codablecash {

SyncHeaderRepo::SyncHeaderRepo(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;

}

SyncHeaderRepo::~SyncHeaderRepo() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void SyncHeaderRepo::createBlankDatabase() {
	{
		UnicodeString fileName(MAIN_DATA);

		BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
		SyncHeaderDataFactory* dataFactory = new SyncHeaderDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void SyncHeaderRepo::open() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	SyncHeaderDataFactory* dataFactory = new SyncHeaderDataFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void SyncHeaderRepo::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

void SyncHeaderRepo::add(const SyncHeaderHeightData *headerData) {
	HeightHeaderSyncDataList dataList;

	{
		const ArrayList<BlockHeaderTransferData>* list = headerData->getDataList();

		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			BlockHeaderTransferData* data = list->get(i);

			dataList.addData(data);
		}
	}

	uint64_t height = headerData->getHeight();
	ULongKey key(height);

	this->dataStore->putData(&key, &dataList);
}

HeightHeaderSyncDataList* SyncHeaderRepo::getFirst() const {
	HeightHeaderSyncDataList* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		ret = dynamic_cast<HeightHeaderSyncDataList*>(obj->copyData());
	}

	return ret;
}

ULongKey* SyncHeaderRepo::getFirstKey() const {
	ULongKey* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const AbstractBtreeKey* obj = scanner->nextKey();
		ret = dynamic_cast<ULongKey*>(obj->copyData());
	}

	return ret;
}


bool SyncHeaderRepo::isEmpty() const {
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);
	scanner->begin();

	return !scanner->hasNext();
}

HeightHeaderSyncDataList* SyncHeaderRepo::fetchFirst() const {
	ULongKey* key = getFirstKey(); __STP(key);

	IBlockObject* obj = this->dataStore->findByKey(key); __STP(obj);
	this->dataStore->remove(key);

	return dynamic_cast<HeightHeaderSyncDataList*>(__STP_MV(obj));
}

} /* namespace codablecash */
