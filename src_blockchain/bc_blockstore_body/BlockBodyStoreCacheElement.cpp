/*
 * BlockBodyStoreCacheElement.cpp
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#include "bc_blockstore_body/BlockBodyStoreCacheElement.h"
#include "bc_blockstore_body/BlockBodyStore.h"
#include "bc_blockstore_body/BlockMerkleRootIndex.h"

#include "osenv/funcs.h"

#include "random_access_file/DiskCacheManager.h"

#include "base_io/File.h"

#include "bc_block_body/BlockBody.h"

namespace codablecash {

BlockBodyStoreCacheElement::BlockBodyStoreCacheElement(const File* baseDir, uint64_t index) {
	this->baseDir = new File(*baseDir);
	this->index = index;

	this->cacheManager = new DiskCacheManager(Os::getSystemPageSize() * 4 * 4);
	this->store = nullptr;
	this->merkleRootIndex = nullptr;
}

BlockBodyStoreCacheElement::~BlockBodyStoreCacheElement() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void BlockBodyStoreCacheElement::init() {
	this->store = new BlockBodyStore(this->index, this->baseDir, this->cacheManager);
	this->merkleRootIndex = new BlockMerkleRootIndex(this->index, this->baseDir, this->cacheManager);

	this->store->open();
	this->merkleRootIndex->open();
}

bool BlockBodyStoreCacheElement::exist() const noexcept {
	BlockBodyStore s(this->index, this->baseDir, this->cacheManager);
	BlockMerkleRootIndex h(this->index, this->baseDir, this->cacheManager);

	bool storeexist = s.exists();
	bool indexexists = h.exists();

	return storeexist && indexexists;
}

void BlockBodyStoreCacheElement::create() {
	BlockBodyStore s(this->index, this->baseDir, this->cacheManager);
	BlockMerkleRootIndex h(this->index, this->baseDir, this->cacheManager);

	s.create();
	h.create();
}

void BlockBodyStoreCacheElement::close() {
	if(this->store != nullptr){
		this->store->close();
		delete this->store, this->store = nullptr;
	}
	if(this->merkleRootIndex != nullptr){
		this->merkleRootIndex->close();
		delete this->merkleRootIndex, this->merkleRootIndex = nullptr;
	}
}

void BlockBodyStoreCacheElement::addBody(const BlockBody *body) {
	uint64_t fpos = this->store->storeBody(body);

	const BlockMerkleRoot* root = body->getMerkleRoot();
	this->merkleRootIndex->addBody(root, fpos);
}

BlockBody* BlockBodyStoreCacheElement::getBlockBody(	const BlockMerkleRoot *root) {
	uint64_t fpos = this->merkleRootIndex->getBodyFpos(root);
	return fpos != 0 ? this->store->loadBody(fpos) : nullptr;
}

void BlockBodyStoreCacheElement::removeBlock(const BlockMerkleRoot *root, uint64_t height) {
	uint64_t fpos = this->merkleRootIndex->getBodyFpos(root);
	this->merkleRootIndex->removeBody(root);

	this->store->removeBody(fpos);
}

} /* namespace codablecash */
