/*
 * UtxoData.cpp
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#include "bc_base_utxo_index/UtxoData.h"

#include "base/StackRelease.h"

#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/TransactionId.h"

#include "bc_block/BlockHeaderId.h"

namespace codablecash {

UtxoData::UtxoData(const AbstractUtxo* utxo, const TransactionId* trxId, uint64_t height, const BlockHeaderId* headerId) {
	this->utxo = dynamic_cast<AbstractUtxo*>(utxo->copyData());
	this->trxId = dynamic_cast<TransactionId*>(trxId->copyData());
	this->height = height;
	this->headerId = headerId != nullptr ? dynamic_cast<BlockHeaderId*>(headerId->copyData()) : nullptr;
	this->removed = false;
}

UtxoData::~UtxoData() {
	delete this->utxo;
	delete this->trxId;
	delete this->headerId;
}

int UtxoData::binarySize() const {
	int total = this->utxo->binarySize();
	total += this->trxId->binarySize();
	total += sizeof(this->height);

	total += sizeof(uint8_t);
	if(this->headerId != nullptr){
		total += this->headerId->binarySize();
	}

	total += sizeof(uint8_t); // removed

	return total;
}

void UtxoData::toBinary(ByteBuffer *out) const {
	this->utxo->toBinary(out);
	this->trxId->toBinary(out);
	out->putLong(this->height);

	bool isnotNull = (this->headerId != nullptr);
	out->put(isnotNull ? 1 : 0);

	if(isnotNull){
		this->headerId->toBinary(out);
	}

	out->put(this->removed ? 1 : 0); // removed
}

UtxoData* UtxoData::fromBinary(ByteBuffer *in) {
	AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in); __STP(utxo);
	TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);
	uint64_t height = in->getLong();

	BlockHeaderId* headerId = nullptr;
	uint8_t bl = in->get();
	bool isnotNull = (bl > 0);
	if(isnotNull){
		headerId = BlockHeaderId::fromBinary(in);
	}
	 __STP(headerId);

	 UtxoData* data = new UtxoData(utxo, trxId, height, headerId); __STP(data);

	 bl = in->get();
	 data->removed = (bl > 0); // removed

	return __STP_MV(data);
}

IBlockObject* UtxoData::copyData() const noexcept {
	UtxoData* data = new UtxoData(this->utxo, this->trxId, this->height, this->headerId);
	data->removed = this->removed;

	return data;
}

} /* namespace codablecash */
