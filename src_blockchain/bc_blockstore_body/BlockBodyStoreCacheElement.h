/*
 * BlockBodyStoreCacheElement.h
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHEELEMENT_H_
#define BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHEELEMENT_H_

namespace alinous {
class File;
class DiskCacheManager;
}
using namespace alinous;

#include <cstdint>

#include "base/ArrayList.h"

namespace codablecash {

class BlockBodyStore;
class BlockBody;
class BlockMerkleRoot;
class BlockMerkleRootIndex;

class BlockBodyStoreCacheElement {
public:
	BlockBodyStoreCacheElement(const File* baseDir, uint64_t index);
	virtual ~BlockBodyStoreCacheElement();

	void init();

	bool exist() const noexcept;
	void create();

	void close();

	void addBody(const BlockBody* body);
	BlockBody* getBlockBody(const BlockMerkleRoot* root);
	void removeBlock(const BlockMerkleRoot* root, uint64_t height);


	uint64_t getIndex() const noexcept {
		return index;
	}

private:
	uint64_t index;
	File* baseDir;
	DiskCacheManager* cacheManager;

	BlockBodyStore* store;
	BlockMerkleRootIndex* merkleRootIndex;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHEELEMENT_H_ */
