/*
 * BlockHead.h
 *
 *  Created on: 2023/03/19
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_BLOCKHEAD_H_
#define BC_STATUS_CACHE_BLOCKHEAD_H_

#include "base/ArrayList.h"

#include <cstdint>

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class BlockHeader;
class BlockHeadElement;
class ISystemLogger;

class BlockHead {
public:
	BlockHead(const BlockHead& inst) = delete;
	BlockHead();
	virtual ~BlockHead();

	BlockHead* copyOriginalBlockHead() const noexcept;

	void add(const BlockHeader* header) noexcept;
	void add(const BlockHeadElement* element) noexcept;

	const BlockHeader* getHeadHeader() const noexcept;
	uint64_t getHeadHeight() const noexcept;
	const BlockHeader* getRealHeadHeader() const noexcept;

	const ArrayList<BlockHeadElement>* getHeaders() const noexcept {
		return &this->list;
	}
	const BlockHeadElement* getHeadBlockHead() const noexcept;

	int size() const noexcept {
		return this->list.size();
	}
	void normalizeWithlength(int length);

	void logStatus(ISystemLogger* logger) const;

	int compareTo(const BlockHead* other) const noexcept;

private:
	BlockHeadElement* getTopBlockHeadElement(int posFromTop) const noexcept;

	int compareToByVoted(const BlockHead* other) const noexcept;
	int compareToByVoting(const BlockHead* other) const noexcept;
	int compareToByTotalMevScore(const BlockHead* other) const noexcept;
	int compareToByLastHeight(const BlockHead* other) const noexcept;
	int compareToByLastTimestamp(const BlockHead* other) const noexcept;

private:
	ArrayList<BlockHeadElement> list;
};

class BlockHeadCompare {
public:
	int operator() (const BlockHead* const a, const BlockHead* const b) const noexcept;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_BLOCKHEAD_H_ */
