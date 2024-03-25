/*
 * AbstractContainerHashAlgorithmElement.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_ABSTRACTCONTAINERHASHALGORITHMELEMENT_H_
#define POW_HASH_ABSTRACTCONTAINERHASHALGORITHMELEMENT_H_

#include "pow_hash/AbstractHashAlgorithmElement.h"

#include "base/ArrayList.h"

using alinous::ArrayList;

namespace codablecash {

class AbstractContainerHashAlgorithmElement : public AbstractHashAlgorithmElement {
public:
	explicit AbstractContainerHashAlgorithmElement();
	virtual ~AbstractContainerHashAlgorithmElement();

	void addOperand(AbstractHashAlgorithmElement* ope) noexcept;

	virtual void buildInputs(const BigInteger* nonce, int pos) noexcept;

protected:
	ArrayList<AbstractHashAlgorithmElement>* list;
};

} /* namespace codablecash */

#endif /* POW_HASH_ABSTRACTCONTAINERHASHALGORITHMELEMENT_H_ */
