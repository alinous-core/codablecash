/*
 * UtxoIdKey.h
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#ifndef BC_BASE_UTXO_INDEX_UTXOIDKEY_H_
#define BC_BASE_UTXO_INDEX_UTXOIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class UtxoId;

class UtxoIdKey : public AbstractBtreeKey {
public:
	UtxoIdKey();
	explicit UtxoIdKey(const UtxoId* utxoId);
	virtual ~UtxoIdKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static UtxoIdKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	const UtxoId* getUtxoId() const noexcept {
		return this->utxoId;
	}

private:
	UtxoId* utxoId;
};

} /* namespace codablecash */

#endif /* BC_BASE_UTXO_INDEX_UTXOIDKEY_H_ */
