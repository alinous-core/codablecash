/*
 * CodablecashBlockchain.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_CODABLECASHBLOCKCHAIN_H_
#define BC_BLOCKSTORE_CODABLECASHBLOCKCHAIN_H_

#include "base/ArrayList.h"

#include <cstdint>


namespace alinous {
class File;
class UnicodeString;
class ConcurrentGate;
class SysMutex;
}
using namespace alinous;


namespace codablecash {

class ZoneStore;
class StatusStore;
class SoftwareVersion;
class Block;
class BlockHeader;
class IBlockchainEventListner;
class BlockHeaderStoreManager;
class BlockBodyStoreManager;
class CentralProcessor;
class BlockHeaderId;
class MemPoolTransaction;

class CodablecashBlockchain {
public:
	static constexpr int DEFAULT_SECTION_LIMIT{10000};

	static const UnicodeString CONFIG_BIN_FILE;

	static const UnicodeString KEY_SECTION_LIMIT;
	static const UnicodeString KEY_ZONE_SELF;
	static const UnicodeString KEY_NUM_ZONES;
	static const UnicodeString KEY_BLOCK_VERSION_MAJOR;
	static const UnicodeString KEY_BLOCK_VERSION_MINOR;
	static const UnicodeString KEY_BLOCK_VERSION_PATCH;

private:
	CodablecashBlockchain(const File* baseDir, uint16_t zoneSelf, uint16_t numZones);
	void initBlankchain();
	void initZonesStore();
	void initZone(uint16_t zone);

public:
	explicit CodablecashBlockchain(const File* baseDir);
	virtual ~CodablecashBlockchain();

	static void createBlankBlockchain(const File* baseDir, uint16_t zoneSelf, uint16_t numZones);

	void open();
	void close() noexcept;

	void addBlock(MemPoolTransaction* memTrx, const Block* block);
	void addHeader(MemPoolTransaction* memTrx, const BlockHeader* header);

	ArrayList<Block>* getBlocksHeightAt(uint16_t zone, uint64_t height) const;

	void cleanOnFinalize(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId, uint16_t lastFinalizedHeight);

	void addListner(IBlockchainEventListner* listner) noexcept;

	BlockHeaderStoreManager* getHeaderManager(uint16_t zone) const noexcept;
	BlockBodyStoreManager* getBlockBodyStoreManager(uint16_t zone) const noexcept;

	uint16_t getZoneSelf() const noexcept {
		return this->zoneSelf;
	}
	int getNumZones() const noexcept {
		return this->zonesStore->size();
	}

	void setProcessor(CentralProcessor* processor) noexcept {
		this->processor = processor;
	}
	CentralProcessor* getProcessor() const noexcept {
		return this->processor;
	}

	void firePostBlockAdded(const Block* block);

private:
	void fireBlockAdded(MemPoolTransaction* memTrx, const Block* block);
	void fireBlockHeaderAdded(MemPoolTransaction* memTrx, const BlockHeader* header);

	void saveCondig();
	void loadCondig();

private:
	File* baseDir;
	SoftwareVersion* version;

	int sectionLimit;

	uint16_t zoneSelf;
	uint16_t numZones;

	StatusStore* configStore;

	ArrayList<ZoneStore>* zonesStore;

	ConcurrentGate* rwLock;

	SysMutex* listnerLock;
	ArrayList<IBlockchainEventListner> listners;
	CentralProcessor* processor;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_CODABLECASHBLOCKCHAIN_H_ */
