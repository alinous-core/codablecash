/*
 * HeadBlockDetectorCacheElement.h
 *
 *  Created on: 2023/05/25
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_HEADBLOCKDETECTORCACHEELEMENT_H_
#define BC_STATUS_CACHE_HEADBLOCKDETECTORCACHEELEMENT_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class TransactionId;
class BlockHeadElement;
class MemPoolTransaction;

class HeadBlockDetectorCacheElement {
public:
	HeadBlockDetectorCacheElement();
	virtual ~HeadBlockDetectorCacheElement();

	void consume(const TransactionId* trxId) noexcept;

	void importBlockHeadElement(const BlockHeadElement* element);

	void export2BlockHeadElement(BlockHeadElement* element, MemPoolTransaction* memTransaction) const noexcept;

private:
	ArrayList<TransactionId> list;

	int voterScore;
	int votingScore;
	int mevHealthScore;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_HEADBLOCKDETECTORCACHEELEMENT_H_ */
