/*
 * ZoneStore.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_ZONESTORE_H_
#define BC_BLOCKSTORE_ZONESTORE_H_

#include <cstdint>

#include "bc_blockstore_header/IHeaderRemovalNotifier.h"

namespace alinous {
class File;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderStoreManager;
class BlockBodyStoreManager;
class Block;
class BlockHeaderId;

class ZoneStore : public IHeaderRemovalNotifier {
public:
	ZoneStore(uint16_t zone, File* baseDir, bool headerOnly, int sectionLimit);
	virtual ~ZoneStore();

	void open();
	void close() noexcept;

	void addBlock(const Block* block);
	void addBlockHeader(const BlockHeader* header);

	BlockHeaderStoreManager* getBlockHeaderStoreManager() const noexcept {
		return this->headerStore;
	}
	BlockBodyStoreManager* getBlockBodyStoreManager() const noexcept {
		return this->bodyStore;
	}

	void cleanOnFinalize(uint64_t finalizingHeight, const BlockHeaderId *finaliZingHeaderId, uint16_t lastFinalizedHeight);
	virtual void onRemovedHeader(const BlockHeader* header) noexcept; // call back

private:
	uint16_t zone;

	File* baseDir;
	bool headerOnly;

	int sectionLimit;
	BlockHeaderStoreManager* headerStore;
	BlockBodyStoreManager* bodyStore;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_ZONESTORE_H_ */
