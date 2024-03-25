/*
 * BlockHeadElementPadding.h
 *
 *  Created on: 2023/12/18
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_BLOCKHEADELEMENTPADDING_H_
#define BC_STATUS_CACHE_BLOCKHEADELEMENTPADDING_H_

#include "bc_status_cache/BlockHeadElement.h"

namespace codablecash {

class BlockHeadElementPadding : public BlockHeadElement {
public:
	BlockHeadElementPadding();
	virtual ~BlockHeadElementPadding();

	virtual bool isPaddong() const noexcept {
		return true;
	}
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_BLOCKHEADELEMENTPADDING_H_ */
