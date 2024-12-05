/*
 * VotingBlockStatusRepository.cpp
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/VotingBlockStatusRepository.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatusDataFactory.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"

#include "random_access_file/DiskCacheManager.h"

#include "base_io/File.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeScanner.h"

#include "base/StackRelease.h"
#include "base/ArrayList.h"

#include "bc_blockstore_header/BlockHeaderIdKeyFactory.h"
#include "bc_blockstore_header/BlockHeaderIdKey.h"

#include "bc_block/BlockHeaderId.h"


namespace codablecash {

VotingBlockStatusRepository::VotingBlockStatusRepository(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->voteStatusStore = nullptr;
}

VotingBlockStatusRepository::~VotingBlockStatusRepository() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void VotingBlockStatusRepository::init() {
	{
		UnicodeString fileName(NAME_VOTE_STATUS_STORE);

		BlockHeaderIdKeyFactory* keyFactory = new BlockHeaderIdKeyFactory(); __STP(keyFactory);
		VotingBlockStatusDataFactory* dataFactory = new VotingBlockStatusDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void VotingBlockStatusRepository::open() {
	{
		UnicodeString fileName(NAME_VOTE_STATUS_STORE);

		BlockHeaderIdKeyFactory* keyFactory = new BlockHeaderIdKeyFactory(); __STP(keyFactory);
		VotingBlockStatusDataFactory* dataFactory = new VotingBlockStatusDataFactory(); __STP(dataFactory);

		this->voteStatusStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->voteStatusStore->open(&opconf);
	}
}

void VotingBlockStatusRepository::close() {
	__close(&this->voteStatusStore);
}

void VotingBlockStatusRepository::__close(Btree **tree) noexcept {
	if(*tree != nullptr){
		(*tree)->close();
		delete (*tree);
		(*tree) = nullptr;
	}
}

void VotingBlockStatusRepository::storeVotingBlockStatus(const VotingBlockStatus *status) {
	const BlockHeaderId* headerId = status->getBlockHeaderId();
	BlockHeaderIdKey key(headerId);

	this->voteStatusStore->putData(&key, status);
}

VotingBlockStatus* VotingBlockStatusRepository::getVotingBlockStatus(const BlockHeaderId *blockHeaderId) {
	BlockHeaderIdKey key(blockHeaderId);

	IBlockObject* obj = this->voteStatusStore->findByKey(&key);

	return dynamic_cast<VotingBlockStatus*>(obj);
}

BtreeScanner* VotingBlockStatusRepository::getBtreeScanner() {
	BtreeScanner* scanner = this->voteStatusStore->getScanner();
	scanner->begin();

	return scanner;
}

void VotingBlockStatusRepository::clean(uint64_t height) {
	ArrayList<BlockHeaderIdKey> list;
	list.setDeleteOnExit();

	{
		BtreeScanner* scanner = this->voteStatusStore->getScanner(); __STP(scanner);
		scanner->begin();

		while(scanner->hasNext()){
			const AbstractBtreeKey* key = scanner->nextKey();
			const IBlockObject* obj = scanner->next();

			const VotingBlockStatus* status = dynamic_cast<const VotingBlockStatus*>(obj);
			uint64_t h = status->getHeight();
			if(h < height){
				BlockHeaderIdKey* headerKey = dynamic_cast<BlockHeaderIdKey*>(key->copyData());
				list.addElement(headerKey);
			}
		}
	}

	{
		int maxLoop = list.size();
		for(int i = 0; i != maxLoop; ++i){
			BlockHeaderIdKey* key = list.get(i);
			bool bl = this->voteStatusStore->remove(key);

			assert(bl == true);
		}
	}
}

} /* namespace codablecash */
