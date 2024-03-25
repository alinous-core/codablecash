/*
 * IBlockchainEventListner.h
 *
 *  Created on: 2023/03/17
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_IBLOCKCHAINEVENTLISTNER_H_
#define BC_BLOCKSTORE_IBLOCKCHAINEVENTLISTNER_H_

namespace codablecash {

class Block;
class BlockHeader;
class CodablecashBlockchain;
class MemPoolTransaction;

class IBlockchainEventListner {
public:
	IBlockchainEventListner();
	virtual ~IBlockchainEventListner();

	virtual void onBlockAdded(MemPoolTransaction* memTrx, const Block* block, CodablecashBlockchain* chain) = 0;
	virtual void postBlockAdded(const Block* block, CodablecashBlockchain* chain) = 0;
	virtual void onBlockHeaderAdded(MemPoolTransaction* memTrx, const BlockHeader* block, CodablecashBlockchain* chain) = 0;
	//virtual void postBlockHeaderAdded(const BlockHeader* header, CodablecashBlockchain* chain) = 0;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_IBLOCKCHAINEVENTLISTNER_H_ */
