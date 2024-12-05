/*
 * VoteManager.cpp
 *
 *  Created on: 2024/04/29
 *      Author: iizuka
 */

#include "bc_status_cache_vote/VoteManager.h"
#include "bc_status_cache_vote/HeightVoteDataFactory.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "bc_block/BlockHeader.h"

#include "bc_status_cache_vote/HeightVoteData.h"

#include "base_timestamp/SystemTimestamp.h"
#include "bc/CodablecashSystemParam.h"
namespace codablecash {

VoteManager::VoteManager(const File* baseDir) {
	this->baseDir = baseDir->get(VOTE_HEIGHT_DATA);
	this->mutex = new SysMutex();
	this->cacheManager = new DiskCacheManager();
	this->heightVoteDataIndex = nullptr;
}

VoteManager::~VoteManager() {
	close();

	delete this->baseDir;
	delete this->mutex;
	delete this->cacheManager;
}

void VoteManager::initBlank() {
	UnicodeString fileName(VOTE_HEIGHT_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	HeightVoteDataFactory* dataFactory = new HeightVoteDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void VoteManager::open() {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	UnicodeString fileName(VOTE_HEIGHT_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	HeightVoteDataFactory* dataFactory = new HeightVoteDataFactory(); __STP(dataFactory);

	this->heightVoteDataIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->heightVoteDataIndex->open(&opconf);
}

void VoteManager::close() noexcept {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	if(this->heightVoteDataIndex != nullptr){
		this->heightVoteDataIndex->close();
		delete this->heightVoteDataIndex, this->heightVoteDataIndex = nullptr;
	}
}

bool VoteManager::registerBlockHeader(const BlockHeader *header, const CodablecashSystemParam *param) {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);

	uint64_t currentHeight = header->getHeight();

	uint64_t voteBeforeNBlocks = param->getVoteBeforeNBlocks(currentHeight);

	uint64_t votedHeight = currentHeight - 1 - voteBeforeNBlocks;

	bool updated = false;
	if(votedHeight > 0){
		HeightVoteData* data = getHeightVoteData(votedHeight); __STP(data);
		const SystemTimestamp* tm = header->getNonceGeneratedtimestamp();

		updated = updateLimitTime(votedHeight, data, tm);
	}

	return updated;
}

HeightVoteData* VoteManager::getHeightVoteData(uint64_t height) {
	ULongKey key(height);

	IBlockObject* obj = this->heightVoteDataIndex->findByKey(&key); __STP(obj);
	if(obj == nullptr){
		HeightVoteData* newobj = new HeightVoteData();
		__STP_RESET(obj, newobj);
		obj = newobj;


		this->heightVoteDataIndex->putData(&key, newobj);
	}

	HeightVoteData* ret = dynamic_cast<HeightVoteData*>(__STP_MV(obj));
	assert(ret != nullptr);

	return ret;
}

bool VoteManager::updateLimitTime(uint64_t height, HeightVoteData *data, const SystemTimestamp *limit) {
	bool updated = false;

	const SystemTimestamp* current = data->getTimestamp();
	if(current == nullptr || current->compareTo(limit) > 0){
		ULongKey key(height);

		data->setTimestamp(limit);
		data->validateTransactions();

		this->heightVoteDataIndex->putData(&key, data);
		updated = true;
	}

	return updated;
}

} /* namespace codablecash */
