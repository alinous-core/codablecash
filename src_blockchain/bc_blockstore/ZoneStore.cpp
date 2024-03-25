/*
 * ZoneStore.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_blockstore/ZoneStore.h"
#include "base_io/File.h"
#include "bc_blockstore_body/BlockBodyStoreManager.h"
#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "base/StackRelease.h"

#include "bc_block/BlockHeaderId.h"
namespace codablecash {

ZoneStore::ZoneStore(uint16_t zone, File* baseDir, bool headerOnly, int sectionLimit) {
	this->zone = zone;
	this->headerOnly = headerOnly;
	this->sectionLimit = sectionLimit;

	this->baseDir = new File(*baseDir);

	this->bodyStore = nullptr;
	this->headerStore = nullptr;
}

ZoneStore::~ZoneStore() {
	close();

	delete this->baseDir;
}

void ZoneStore::open() {
	if(!this->headerOnly){
		this->bodyStore = new BlockBodyStoreManager(this->baseDir, this->sectionLimit);
	}

	this->headerStore = new BlockHeaderStoreManager(this->baseDir, this->sectionLimit);
}

void ZoneStore::close() noexcept {
	delete this->headerStore;
	this->headerStore = nullptr;

	delete this->bodyStore;
	this->bodyStore = nullptr;
}

void ZoneStore::addBlock(const Block *block) {
	const BlockBody* body = block->getBody();
	const BlockHeader* header = block->getHeader();

	this->bodyStore->addBlockBody(body, header->getHeight());
	this->headerStore->addHeader(header);
}
void ZoneStore::addBlockHeader(const BlockHeader *header) {
	this->headerStore->addHeader(header);
}

void ZoneStore::cleanOnFinalize(uint64_t finalizingHeight,	const BlockHeaderId *finaliZingHeaderId, uint16_t lastFinalizedHeight) {
	const BlockHeaderId* headerId = dynamic_cast<const BlockHeaderId*>(finaliZingHeaderId->copyData());
	uint64_t height = finalizingHeight;

	do{
		__STP(headerId);
		BlockHeader* header = this->headerStore->getHeader(headerId, height); __STP(header);

		this->headerStore->finalize(height, headerId, this);

		// prepare for next loop
		headerId = dynamic_cast<const BlockHeaderId*>(header->getLastHeaderId()->copyData());
		height--;
	}
	while(height > lastFinalizedHeight);
	__STP(headerId);
}

void ZoneStore::onRemovedHeader(const BlockHeader *header) noexcept {
	if(this->bodyStore != nullptr){
		const BlockMerkleRoot* root = header->getMerkleRoot();
		uint64_t height = header->getHeight();

		this->bodyStore->removeBlockBody(root, height);
	}
}


} /* namespace codablecash */
