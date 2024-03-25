/*
 * BlockBodyStore.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BLOCKBODYSTOREMANAGER_H_
#define BC_BLOCKSTORE_BLOCKBODYSTOREMANAGER_H_

#include "base/ArrayList.h"
#include <cstdint>

namespace alinous {
class File;
class ConcurrentGate;
}
using namespace alinous;

namespace codablecash {

class BlockBodyStoreCache;
class BlockBody;
class BlockMerkleRoot;

class BlockBodyStoreManager {
public:
	static const constexpr wchar_t* MAIN_BODY_DIR{L"mainheader"};

	BlockBodyStoreManager(const File* baseDir, int sectionLimit);
	virtual ~BlockBodyStoreManager();

	void addBlockBody(const BlockBody* body, uint64_t height);
	BlockBody* getBlockBody(const BlockMerkleRoot* root, uint64_t height);
	void removeBlockBody(const BlockMerkleRoot* root, uint64_t height);

private:
	File* baseDir;
	int sectionLimit;

	BlockBodyStoreCache* cache;

	ConcurrentGate* rwLock;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BLOCKBODYSTOREMANAGER_H_ */
