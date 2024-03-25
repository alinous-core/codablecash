/*
 * PoWForLoop.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_POWFORLOOP_H_
#define POW_HASH_POWFORLOOP_H_

#include "pow_hash/AbstractContainerHashAlgorithmElement.h"

namespace codablecash {

class PoWForLoop : public AbstractContainerHashAlgorithmElement {
public:
	explicit PoWForLoop();
	virtual ~PoWForLoop();

	void setLoops(int count) noexcept {
		this->loops = count;
	}

protected:
	virtual void calulate(const char* in01, const char* in02, const char* in03, char* out) const noexcept;

private:
	int loops;
};

} /* namespace codablecash */

#endif /* POW_HASH_POWFORLOOP_H_ */
