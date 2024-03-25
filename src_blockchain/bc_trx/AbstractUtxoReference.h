/*
 * AbstractUtxoReference.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_TRX_ABSTRACTUTXOREFERENCE_H_
#define BC_TRX_ABSTRACTUTXOREFERENCE_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class UtxoId;

class AbstractUtxoReference : public alinous::IBlockObject {
public:
	static const constexpr uint8_t UTXO_REF_TYPE_BALANCE{1};
	static const constexpr uint8_t UTXO_REF_TYPE_UTXO_TICKET{2};

	static const constexpr uint8_t UTXO_REF_TYPE_UTXO_VOTED_TICKET{3};

	static const constexpr uint8_t UTXO_REF_TYPE_COINBASE{4};
	static const constexpr uint8_t UTXO_REF_TYPE_STAKEBASE{5};


	AbstractUtxoReference(const AbstractUtxoReference& inst);
	AbstractUtxoReference();
	virtual ~AbstractUtxoReference();

	static AbstractUtxoReference* createFromBinary(ByteBuffer* in);

	virtual uint8_t getType() const noexcept = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	const UtxoId* getUtxoId() const noexcept {
		return this->utxoId;
	}
	void setUtxoId(const UtxoId* utxoId) noexcept;

protected:
	UtxoId* utxoId;
};

} /* namespace codablecash */

#endif /* BC_TRX_ABSTRACTUTXOREFERENCE_H_ */
