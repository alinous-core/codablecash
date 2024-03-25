/*
 * UtxoData.h
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#ifndef BC_BASE_UTXO_INDEX_UTXODATA_H_
#define BC_BASE_UTXO_INDEX_UTXODATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AbstractUtxo;
class TransactionId;
class BlockHeaderId;

class UtxoData : public IBlockObject {
public:
	UtxoData(const UtxoData& inst) = delete;
	explicit UtxoData(const AbstractUtxo* utxo, const TransactionId* trxId, uint64_t height, const BlockHeaderId* headerId);
	virtual ~UtxoData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static UtxoData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const AbstractUtxo* getUtxo() const noexcept {
		return this->utxo;
	}

	TransactionId* getTransactionId() const noexcept {
		return this->trxId;
	}

	uint64_t getHeight() const noexcept {
		return this->height;
	}

	const BlockHeaderId* getBlockHeaderId() const noexcept {
		return this->headerId;
	}

	void setRemoved(bool removed) noexcept {
		this->removed = removed;
	}

	bool isRemoved() const noexcept {
		return this->removed;
	}

private:
	AbstractUtxo* utxo;
	TransactionId* trxId;
	uint64_t height;
	BlockHeaderId* headerId;
	bool removed;
};

} /* namespace codablecash */

#endif /* BC_BASE_UTXO_INDEX_UTXODATA_H_ */
