/*
 * HeadBlockDetector.cpp
 *
 *  Created on: 2023/03/19
 *      Author: iizuka
 */

#include "bc_status_cache/HeadBlockDetector.h"
#include "bc_status_cache/BlockHead.h"
#include "bc_status_cache/BlockHeadElement.h"
#include "bc_status_cache/BlockStack.h"
#include "bc_status_cache/HeadBlockDetectorCacheElement.h"
#include "bc_status_cache/HeadBlockDetectorCache.h"
#include "bc_status_cache/BlockStackElement.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"
#include "bc_block/BlockHeaderId.h"

#include "bc_block_body/BlockBody.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_blockstore_body/BlockBodyStoreManager.h"

#include "base/StackRelease.h"
#include "bc/CodablecashSystemParam.h"

#include "bc_memorypool/MemPoolTransaction.h"

#include "bc/ISystemLogger.h"

#include "bc_trx/AbstractControlTransaction.h"
#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx/AbstractInterChainCommunicationTansaction.h"

#include "transaction/AbstractSmartcontractTransaction.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"

#include "bc_block/BlockMerkleRoot.h"

namespace codablecash {

HeadBlockDetector::HeadBlockDetector(ISystemLogger* logger) {
	this->logger = logger;
	this->cache = new HeadBlockDetectorCache();
	this->selectedHead = nullptr;
	this->secondHead = nullptr;
	this->scheduledBlock = nullptr;
}

HeadBlockDetector::~HeadBlockDetector() {
	reset();

	delete this->scheduledBlock;
	this->scheduledBlock = nullptr;

	delete this->cache;
}

void HeadBlockDetector::reset() noexcept {
	this->headsList.deleteElements();
	this->headsList.reset();

	delete this->selectedHead;
	this->selectedHead = nullptr;

	delete this->secondHead;
	this->secondHead = nullptr;

	this->cache->reset();
}

void HeadBlockDetector::setScheduledBlock(const Block *block) {
	delete this->scheduledBlock;
	this->scheduledBlock = new Block(*block);
}

Block* HeadBlockDetector::fetchScheduledBlock() {
	Block* ret = nullptr;

	if(this->scheduledBlock != nullptr){
		ret = new Block(*this->scheduledBlock);

		delete this->scheduledBlock;
		this->scheduledBlock = nullptr;
	}

	return ret;
}

Block* HeadBlockDetector::getScheduledBlock() {
	Block* ret = new Block(*this->scheduledBlock);

	return ret;
}

void HeadBlockDetector::buildHeads(uint16_t zone, IBlockchainStoreProvider *chain, uint64_t finalizedHeight) {
	BlockHeaderStoreManager* headerStore = chain->getHeaderManager(zone);

	uint64_t startHeight = finalizedHeight > 0 ? finalizedHeight : 1;
	BlockHeaderId* finalizedTopHeaderId = getFinalizedHeaderId(headerStore, startHeight); __STP(finalizedTopHeaderId);

	BlockStack stack;
	// first root
	{
		ArrayList<BlockHeader>* headerList = headerStore->getBlocksAtHeight(startHeight); __STP(headerList);
		headerList->setDeleteOnExit();

		BlockStackElement* element = new BlockStackElement();
		int maxLoop = headerList->size();
		for(int i = 0; i != maxLoop; ++i){
			const BlockHeader* header = headerList->get(i);
			const BlockHeaderId* headerId = header->getId();


			// filter finalized root
			if(header->getHeight() == 1 || finalizedTopHeaderId->equals(headerId)){
				element->addHeader(header);
			}
		}
		stack.push(element);
	}

	// children
	while(!stack.isEmpty()){
		BlockStackElement* element = stack.top();
		const BlockHeader* header = element->current();
		const BlockHeaderId* headerId = header->getId();
		uint64_t height = header->getHeight();

		// getChildrenOf() does not return nullptr
		ArrayList<BlockHeader>* children = headerStore->getChildrenOf(headerId, height); __STP(children);
		children->setDeleteOnExit();

		// add generated block
		if(this->scheduledBlock != nullptr){
			addScheculedBlock(height, headerId, children);
		}

		if(children->isEmpty()){ // head
			BlockHead* head = stack.createBlockHead();
			this->headsList.addElement(head);

			stack.gotoBranch();
		}
		else{ // add children
			BlockStackElement* element = new BlockStackElement();

			int maxLoop = children->size();
			for(int i = 0; i != maxLoop; ++i){
				const BlockHeader* header = children->get(i);
				element->addHeader(header);
			}

			stack.push(element);
		}
	}

	// normalize
	normalizeHeadLength();
}

void HeadBlockDetector::addScheculedBlock(uint64_t height,	const BlockHeaderId *headerId, ArrayList<BlockHeader> *children) const {
	const BlockHeader* header = this->scheduledBlock->getHeader();
	const BlockHeaderId* scheduledHeaderId = header->getLastHeaderId();
	uint64_t parentHeight = header->getHeight() - 1;

	if(parentHeight == height && headerId->equals(scheduledHeaderId)){
		children->addElement(dynamic_cast<BlockHeader*>(header->copyData()));
	}
}

BlockBody* HeadBlockDetector::getBlockBody(const BlockMerkleRoot *root, uint64_t height, BlockBodyStoreManager* bodyManager) const {
	if(this->scheduledBlock != nullptr){
		const BlockHeader* header = this->scheduledBlock->getHeader();
		uint64_t scheduledHeight = header->getHeight();
		const BlockMerkleRoot* r = header->getMerkleRoot();

		if(scheduledHeight == height && r->equals(root)){
			return dynamic_cast<BlockBody*>(this->scheduledBlock->getBody()->copyData());
		}
	}

	return bodyManager->getBlockBody(root, height);
}

void HeadBlockDetector::normalizeHeadLength() noexcept {
	int length = 0;

	{
		int maxLoop = this->headsList.size();
		for(int i = 0; i != maxLoop; ++i){
			const BlockHead* head = this->headsList.get(i);
			int size = head->size();

			if(size > length){
				length = size;
			}
		}
	}

	{
		int maxLoop = this->headsList.size();
		for(int i = 0; i != maxLoop; ++i){
			BlockHead* head = this->headsList.get(i);

			head->normalizeWithlength(length);
		}
	}
}

BlockHeaderId* HeadBlockDetector::getFinalizedHeaderId(BlockHeaderStoreManager* headerStore, uint64_t finalizedHeight) const {
	if(finalizedHeight == 0){
		return nullptr;
	}
	ArrayList<BlockHeader>* list = headerStore->getBlocksAtHeight(finalizedHeight); __STP(list);
	assert(list != nullptr && list->size() == 1);

	list->setDeleteOnExit();

	return dynamic_cast<BlockHeaderId*>(list->get(0)->getId()->copyData());
}

const BlockHead* HeadBlockDetector::getHead() const noexcept {
	return this->selectedHead;
}

const BlockHead* HeadBlockDetector::getSecondHead() const noexcept {
	return this->secondHead;
}

BlockHeader* HeadBlockDetector::changeMiningTarget() {
	delete this->selectedHead;
	this->selectedHead = this->secondHead;

	this->secondHead = nullptr;

	delete this->scheduledBlock;
	this->scheduledBlock = nullptr;

	const BlockHeader* header = this->selectedHead->getHeadHeader();

	return dynamic_cast<BlockHeader*>(header->copyData());
}

void HeadBlockDetector::selectChain() {
#ifdef __DEBUG__
	{
		int maxLoop = this->headsList.size();
		for(int i = 0; i != maxLoop; ++i){
			BlockHead* head = this->headsList.get(i);

			head->logStatus(this->logger);
		}
	}
#endif

	this->headsList.sort();

	int maxLoop = this->headsList.size();

	delete this->selectedHead;
	this->selectedHead = this->headsList.get(maxLoop - 1)->copyOriginalBlockHead();

	// second
	delete this->secondHead, this->secondHead = nullptr;
	if(maxLoop > 1){
		this->secondHead = this->headsList.get(maxLoop - 2)->copyOriginalBlockHead();
	}

	headsList.deleteElements();
	headsList.reset();

#ifdef __DEBUG__
	{
		UnicodeString message(L"****************** SELECTED *******************");
		UnicodeString message2(L"***********************************************");
		this->logger->debugLog(ISystemLogger::DEBUG_CHAIN_HEAD_DETECT, &message, __FILE__, __LINE__);

		const BlockHead* head = getHead();
		head->logStatus(this->logger);

		this->logger->debugLog(ISystemLogger::DEBUG_CHAIN_HEAD_DETECT, &message2, __FILE__, __LINE__);
	}
#endif

}

void HeadBlockDetector::evaluate(uint16_t zone, MemPoolTransaction *memTrx, IBlockchainStoreProvider *chain, const CodablecashSystemParam *config, bool headerOnly) {

	int maxLoop = this->headsList.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHead* head = this->headsList.get(i);
		evaluateHead(zone, head, memTrx, chain, config, headerOnly);
	}
}

void HeadBlockDetector::evaluateHead(uint16_t zone, BlockHead *head,
		MemPoolTransaction *memTrx, IBlockchainStoreProvider *chain, const CodablecashSystemParam *config, bool headerOnly) {
	MemPoolTransaction* memTransaction = nullptr;
	if(!headerOnly){
		memTransaction = memTrx->newSubTransaction();
	}
	__STP(memTransaction);

	const ArrayList<BlockHeadElement>* list = head->getHeaders();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeadElement* element = list->get(i);
		if(element->isPaddong()){
			break;
		}

		const BlockHeader* header = element->getBlockHeader();
		const BlockHeaderId* headerId = header->getId();
		uint64_t height = header->getHeight();

		// use cache
		{
			const HeadBlockDetectorCacheElement* cacheElement = this->cache->getCache(headerId);
			if(cacheElement != nullptr){
				cacheElement->export2BlockHeadElement(element, memTransaction, headerOnly);
				continue;
			}
		}

		HeadBlockDetectorCacheElement* cacheElemet = new HeadBlockDetectorCacheElement(); __STP(cacheElemet);

		// vote
		handleVotes(config, chain, list, header, i);

		// memory pool
		if(!headerOnly){
			BlockBodyStoreManager* bodyManager = chain->getBlockBodyStoreManager(zone);

			// transactions mev
			handleBody(config, element, bodyManager, memTransaction, cacheElemet);
		}

		// register cache
		cacheElemet->importBlockHeadElement(element);
		this->cache->registerCache(headerId, __STP_MV(cacheElemet));
	}
}

void HeadBlockDetector::handleBody(const CodablecashSystemParam *config, BlockHeadElement* element, BlockBodyStoreManager* bodyManager
		, MemPoolTransaction* memTransaction, HeadBlockDetectorCacheElement* cacheElemet) {
	const BlockHeader* header = element->getBlockHeader();
	uint64_t height = header->getHeight();

	const BlockMerkleRoot* root = header->getMerkleRoot();

	// body from generated block
	BlockBody* body = getBlockBody(root, height, bodyManager); __STP(body);
	assert(body != nullptr);

	{
		const ArrayList<AbstractControlTransaction>* list = body->getControlTransactions();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractControlTransaction* trx = list->get(i);
			const TransactionId* trxId = trx->getTransactionId();


			if(!trx->canAddMempool()){
				continue;
			}
			if(memTransaction->hasControlTransaction(trxId) /*|| !trx->canAddMempool()*/){
				element->hasTransactionInMemoryPool();
			}
			else {
				element->hasNoTransactionInMemoryPool();
			}

			memTransaction->consume(trxId);
			cacheElemet->consume(trxId);
		}
	}
	{
		const ArrayList<AbstractInterChainCommunicationTansaction>* list = body->getInterChainCommunicationTransactions();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractInterChainCommunicationTansaction* trx = list->get(i);
			const TransactionId* trxId = trx->getTransactionId();

			if(memTransaction->hasInterChainCommunicationTansaction(trxId) || !trx->canAddMempool()){
				element->hasTransactionInMemoryPool();
			}
			else {
				element->hasNoTransactionInMemoryPool();
			}

			memTransaction->consume(trxId);
			cacheElemet->consume(trxId);
		}
	}
	{
		const ArrayList<AbstractBalanceTransaction>* list = body->getBalanceTransactions();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractBalanceTransaction* trx = list->get(i);
			const TransactionId* trxId = trx->getTransactionId();

			if(memTransaction->hasBalanceTransaction(trxId) || !trx->canAddMempool()){
				element->hasTransactionInMemoryPool();
			}
			else {
				element->hasNoTransactionInMemoryPool();
			}

			memTransaction->consume(trxId);
			cacheElemet->consume(trxId);
		}
	}
	{
		const ArrayList<AbstractSmartcontractTransaction>* list = body->getSmartcontractTransactions();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractSmartcontractTransaction* trx = list->get(i);
			const TransactionId* trxId = trx->getTransactionId();

			if(memTransaction->hasSmartcontractTransaction(trxId) || !trx->canAddMempool()){
				element->hasTransactionInMemoryPool();
			}
			else {
				element->hasNoTransactionInMemoryPool();
			}

			memTransaction->consume(trxId);
			cacheElemet->consume(trxId);
		}
	}
}

void HeadBlockDetector::handleVotes(const CodablecashSystemParam *config, IBlockchainStoreProvider* chain
		, const ArrayList<BlockHeadElement> *list, const BlockHeader *header, int i) {
	uint64_t height = header->getHeight();

	uint16_t voteBeforeNBlocks = config->getVoteBeforeNBlocks(height);
	uint16_t voteBlockIncludeAfterNBlocks = config->getVoteBlockIncludeAfterNBlocks(height);
	int diffBlocks = voteBeforeNBlocks + voteBlockIncludeAfterNBlocks;

	int pos = i - diffBlocks;
	if(pos >= 0){
		// calc voted score of voted block
		VotePart* vorts = header->getVotePart();
		BlockHeadElement* element = list->get(pos); // voted block

		element->importVotes(vorts);

		// voting
		const BlockHeader* votedHeader = element->getBlockHeader();
		const BlockHeaderId* votedId = votedHeader->getId();
		BlockHeadElement* votingElement = list->get(i); // self block
		votingElement->calcVotingScore(votedId);
		return;
	}


	// can not voted score of voted block
	// calc only voting of this block
	if(height > diffBlocks){
		uint64_t votedHeight = height - diffBlocks;

		uint16_t zone = header->getZone();
		BlockHeaderStoreManager* headerManager = chain->getHeaderManager(zone);

		const BlockHeaderId* votingHeaderId = header->getId();
		BlockHeader* votedHeader = headerManager->getNBlocksBefore(votingHeaderId, height, diffBlocks); __STP(votedHeader);
		const BlockHeaderId* votedId = votedHeader->getId();

		BlockHeadElement* votingElement = list->get(i); // self block
		votingElement->calcVotingScore(votedId);
	}
}

} /* namespace codablecash */
