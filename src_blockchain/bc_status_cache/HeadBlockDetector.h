/*
 * HeadBlockDetector.h
 *
 *  Created on: 2023/03/19
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_HEADBLOCKDETECTOR_H_
#define BC_STATUS_CACHE_HEADBLOCKDETECTOR_H_

#include "base/ArrayList.h"
#include "bc_status_cache/BlockHead.h"
#include "bc_status_cache/IBlockchainStoreProvider.h"

namespace alinous {
class File;
}
using namespace alinous;

#include <cstdint>

namespace codablecash {

class BlockHead;
class CodablecashBlockchain;
class BlockHeaderStoreManager;
class MemPoolTransaction;
class CodablecashSystemParam;
class BlockHeadElement;
class BlockHeader;
class BlockBodyStoreManager;
class BlockHeadCompare;
class BlockHeaderId;
class ISystemLogger;
class HeadBlockDetectorCache;
class HeadBlockDetectorCacheElement;
class Block;
class BlockBody;
class BlockMerkleRoot;
class ZoneStatusCache;

class HeadBlockDetector {
public:
	explicit HeadBlockDetector(ISystemLogger* logger);
	virtual ~HeadBlockDetector();

	void reset() noexcept;

	void buildHeads(uint16_t zone, IBlockchainStoreProvider *chain, uint64_t finalizedHeight);
	void evaluate(uint16_t zone, MemPoolTransaction* memTrx, IBlockchainStoreProvider *chain, const CodablecashSystemParam* config, bool headerOnly);
	void evaluateHead(uint16_t zone, BlockHead* head, MemPoolTransaction* memTrx, IBlockchainStoreProvider *chain
			, const CodablecashSystemParam* config, bool headerOnly);
	void selectChain();

	const BlockHead* getHead() const noexcept;
	const BlockHead* getSecondHead() const noexcept;

	BlockHeader* changeMiningTarget();

	void setScheduledBlock(const Block* block);
	Block* fetchScheduledBlock();

private:
	void addScheculedBlock(uint64_t height, const BlockHeaderId* headerId, ArrayList<BlockHeader>* children) const;
	BlockBody* getBlockBody(const BlockMerkleRoot* root, uint64_t height, BlockBodyStoreManager* bodyManager) const;

	BlockHeaderId* getFinalizedHeaderId(BlockHeaderStoreManager* headerStore, uint64_t finalizedHeight) const;

	void handleVotes(const CodablecashSystemParam *config, IBlockchainStoreProvider* chain
			, const ArrayList<BlockHeadElement>* list, const BlockHeader* header, int i);
	void handleBody(const CodablecashSystemParam *config, BlockHeadElement* element, BlockBodyStoreManager* bodyManager, MemPoolTransaction* memTransaction, HeadBlockDetectorCacheElement* cacheElemet);

	void normalizeHeadLength() noexcept;

private:
	ArrayList<BlockHead, BlockHeadCompare> headsList;
	HeadBlockDetectorCache* cache;

	BlockHead* selectedHead;
	BlockHead* secondHead;

	ISystemLogger* logger;

	Block* scheduledBlock;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_HEADBLOCKDETECTOR_H_ */
