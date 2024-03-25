/*
 * BlockHeaderHeightIndex.cpp
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderHeightIndex.h"
#include "bc_blockstore_header/BlockHeaderHeightIndexKeyFactory.h"
#include "bc_blockstore_header/BlockHeaderHeightIndexDataFactory.h"
#include "bc_blockstore_header/BlockHeaderHeightIndexKey.h"
#include "bc_blockstore_header/BlockHeaderList.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"


namespace codablecash {

BlockHeaderHeightIndex::BlockHeaderHeightIndex(int index, const File* baseDir, DiskCacheManager* cacheManager) {
	this->index = index;
	this->baseDir = new File(*baseDir);
	this->cacheManager = cacheManager;
	this->btree = nullptr;
}

BlockHeaderHeightIndex::~BlockHeaderHeightIndex() {
	close();
	delete this->baseDir;
}

bool BlockHeaderHeightIndex::exists() const noexcept {
	UnicodeString fileName(BlockHeaderHeightIndex::FILE_NAME);
	addIdex2String(&fileName);

	BlockHeaderHeightIndexKeyFactory* keyFactory = new BlockHeaderHeightIndexKeyFactory(); __STP(keyFactory);
	BlockHeaderHeightIndexDataFactory* dataFactory = new BlockHeaderHeightIndexDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	bool ex = btree.exists();

	return ex;
}

void BlockHeaderHeightIndex::create() {
	UnicodeString fileName(BlockHeaderHeightIndex::FILE_NAME);
	addIdex2String(&fileName);

	BlockHeaderHeightIndexKeyFactory* keyFactory = new BlockHeaderHeightIndexKeyFactory(); __STP(keyFactory);
	BlockHeaderHeightIndexDataFactory* dataFactory = new BlockHeaderHeightIndexDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void BlockHeaderHeightIndex::open() {
	UnicodeString fileName(BlockHeaderHeightIndex::FILE_NAME);
	addIdex2String(&fileName);

	BlockHeaderHeightIndexKeyFactory* keyFactory = new BlockHeaderHeightIndexKeyFactory(); __STP(keyFactory);
	BlockHeaderHeightIndexDataFactory* dataFactory = new BlockHeaderHeightIndexDataFactory(); __STP(dataFactory);

	this->btree = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->btree->open(&opconf);
}

void BlockHeaderHeightIndex::close() noexcept {
	if(this->btree != nullptr){
		this->btree->close();
		delete this->btree, this->btree = nullptr;
	}
}

void BlockHeaderHeightIndex::addIdex2String(UnicodeString *str) const noexcept {
	char* buff = new char[32];
	Mem::memset(buff, 0, 32);
	StackArrayRelease<char> __ST_buff(buff);

	::sprintf(buff, "%08d", this->index);
	UnicodeString buffstr(buff);
	str->append(&buffstr);
}

void BlockHeaderHeightIndex::addHeader(uint64_t height, uint64_t fpos) {
	BlockHeaderHeightIndexKey key(height);

	BlockHeaderList headers;
	headers.add(fpos);

	this->btree->putData(&key, &headers);
}

BlockHeaderList* BlockHeaderHeightIndex::getHeadersAtHeight(uint64_t height) {
	BlockHeaderHeightIndexKey key(height);

	IBlockObject* obj = this->btree->findByKey(&key);
	if(obj == nullptr){
		return nullptr;
	}

	BlockHeaderList* list = dynamic_cast<BlockHeaderList*>(obj);

	return list;
}

void BlockHeaderHeightIndex::removeIndex(uint64_t height, uint64_t fpos) {
	BlockHeaderHeightIndexKey key(height, fpos);

	this->btree->remove(&key);
}

} /* namespace codablecash */
