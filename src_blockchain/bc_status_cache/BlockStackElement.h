/*
 * BlockStackElement.h
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_BLOCKSTACKELEMENT_H_
#define BC_STATUS_CACHE_BLOCKSTACKELEMENT_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class BlockHeader;

class BlockStackElement {
public:
	BlockStackElement();
	virtual ~BlockStackElement();

	void addHeader(const BlockHeader* header) noexcept;

	const BlockHeader* current() const noexcept;
	bool hasNext() const noexcept;
	void next() noexcept;

private:
	int pos;
	ArrayList<BlockHeader> list;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_BLOCKSTACKELEMENT_H_ */
