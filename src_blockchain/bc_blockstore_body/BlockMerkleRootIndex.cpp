/*
 * BlockMerkleRootIndex.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_blockstore_body/BlockMerkleRootIndex.h"
#include "bc_blockstore_body/FilePositionDataFactory.h"

#include "bc_block/BlockMerkleRootKey.h"
#include "bc_block/BlockMerkleRootKeyFactory.h"

#include "base_io/File.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "bc_blockstore_body/FilePositionData.h"

namespace codablecash {

BlockMerkleRootIndex::BlockMerkleRootIndex(int index, const File* baseDir, DiskCacheManager* cacheManager) {
	this->index = index;
	this->baseDir = new File(*baseDir);
	this->cacheManager = cacheManager;
	this->btree = nullptr;
}

BlockMerkleRootIndex::~BlockMerkleRootIndex() {
	close();
	delete this->baseDir;
}

bool BlockMerkleRootIndex::exists() const noexcept {
	UnicodeString fileName(FILE_NAME);
	addIdex2String(&fileName);

	BlockMerkleRootKeyFactory* keyFactory = new BlockMerkleRootKeyFactory(); __STP(keyFactory);
	FilePositionDataFactory* dataFactory = new FilePositionDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	bool ex = btree.exists();

	return ex;
}

void BlockMerkleRootIndex::create() {
	UnicodeString fileName(FILE_NAME);
	addIdex2String(&fileName);

	BlockMerkleRootKeyFactory* keyFactory = new BlockMerkleRootKeyFactory(); __STP(keyFactory);
	FilePositionDataFactory* dataFactory = new FilePositionDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void BlockMerkleRootIndex::open() {
	UnicodeString fileName(FILE_NAME);
	addIdex2String(&fileName);

	BlockMerkleRootKeyFactory* keyFactory = new BlockMerkleRootKeyFactory(); __STP(keyFactory);
	FilePositionDataFactory* dataFactory = new FilePositionDataFactory(); __STP(dataFactory);

	this->btree = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->btree->open(&opconf);
}

void BlockMerkleRootIndex::close() noexcept {
	if(this->btree != nullptr){
		this->btree->close();
		delete this->btree, this->btree = nullptr;
	}
}

void BlockMerkleRootIndex::addBody(const BlockMerkleRoot *root, uint64_t fpos) {
	BlockMerkleRootKey key(root);
	FilePositionData data(fpos);

	this->btree->putData(&key, &data);
}

uint64_t BlockMerkleRootIndex::getBodyFpos(const BlockMerkleRoot *root) {
	BlockMerkleRootKey key(root);

	IBlockObject* obj = this->btree->findByKey(&key); __STP(obj);
	FilePositionData* data = dynamic_cast<FilePositionData*>(obj);

	return data != nullptr ? data->getFpos() : 0;
}

void BlockMerkleRootIndex::removeBody(const BlockMerkleRoot *root) {
	BlockMerkleRootKey key(root);

	this->btree->remove(&key);
}

void BlockMerkleRootIndex::addIdex2String(UnicodeString *str) const noexcept {
	char* buff = new char[32];
	Mem::memset(buff, 0, 32);
	StackArrayRelease<char> __ST_buff(buff);

	::sprintf(buff, "%08d", this->index);
	UnicodeString buffstr(buff);
	str->append(&buffstr);
}

} /* namespace codablecash */
