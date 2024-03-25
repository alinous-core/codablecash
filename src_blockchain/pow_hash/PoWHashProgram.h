/*
 * PoWHashProgram.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_POWHASHPROGRAM_H_
#define POW_HASH_POWHASHPROGRAM_H_

#include "pow_hash/AbstractContainerHashAlgorithmElement.h"

namespace codablecash {

class PoWHashProgram : public AbstractContainerHashAlgorithmElement {
public:
	explicit PoWHashProgram();
	virtual ~PoWHashProgram();

	virtual void calulate(const char* in01, const char* in02, const char* in03, char* out) const noexcept;
};

} /* namespace codablecash */

#endif /* POW_HASH_POWHASHPROGRAM_H_ */
