/*
 * FinalizedVoterRepository.cpp
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#include "bc_status_cache_data/FinalizedVoterRepository.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeScanner.h"

#include "bc_finalizer/VoterEntryFactory.h"
#include "bc_finalizer/VoterEntry.h"

#include "bc_network/NodeIdentifierKey.h"
#include "bc_network/NodeIdentifierKeyFactory.h"
#include "bc_network/NodeIdentifier.h"


namespace codablecash {

FinalizedVoterRepository::FinalizedVoterRepository(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->votersStore = nullptr;
}

FinalizedVoterRepository::~FinalizedVoterRepository() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void FinalizedVoterRepository::initBlank() {
	UnicodeString fileName(NAME_VOTER_STORE);

	NodeIdentifierKeyFactory* keyFactory = new NodeIdentifierKeyFactory(); __STP(keyFactory);
	VoterEntryFactory* dataFactory = new VoterEntryFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void FinalizedVoterRepository::open() {
	{
		UnicodeString fileName(NAME_VOTER_STORE);

		NodeIdentifierKeyFactory* keyFactory = new NodeIdentifierKeyFactory(); __STP(keyFactory);
		VoterEntryFactory* dataFactory = new VoterEntryFactory(); __STP(dataFactory);

		this->votersStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->votersStore->open(&opconf);
	}

}

void FinalizedVoterRepository::close() {
	if(this->votersStore != nullptr){
		this->votersStore->close();
		delete this->votersStore, this->votersStore = nullptr;
	}
}

void FinalizedVoterRepository::putVoterEntry(const VoterEntry *entry) {
	NodeIdentifierKey key(entry->getNodeIdentifier());

	this->votersStore->putData(&key, entry);
}

void FinalizedVoterRepository::exportAllToMap(HashMap<NodeIdentifier, VoterEntry> *votersMap) const noexcept {
	BtreeScanner* sacnner = this->votersStore->getScanner(); __STP(sacnner);
	sacnner->begin();

	while(sacnner->hasNext()){
		const IBlockObject* obj = sacnner->next();
		VoterEntry* entry = dynamic_cast<VoterEntry*>(obj->copyData());
		const NodeIdentifier* nodeId = entry->getNodeIdentifier();

		votersMap->put(nodeId, entry);
	}
}

} /* namespace codablecash */
