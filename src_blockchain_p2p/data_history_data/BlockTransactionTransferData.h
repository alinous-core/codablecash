/*
 * BlockTransactionTransferData.h
 *
 *  Created on: Jun 15, 2025
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_BLOCKTRANSACTIONTRANSFERDATA_H_
#define DATA_HISTORY_DATA_BLOCKTRANSACTIONTRANSFERDATA_H_

#include "data_history/AbstractTransferedData.h"

namespace codablecash {

class AbstractBlockchainTransaction;
class MerkleCertificate;
class BlockMerkleRoot;

class BlockTransactionTransferData : public AbstractTransferedData {
public:
	BlockTransactionTransferData(const BlockTransactionTransferData& inst);
	BlockTransactionTransferData();
	virtual ~BlockTransactionTransferData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual TransferedDataId* getTransferedDataId() const;

	void setTransaction(const AbstractBlockchainTransaction* trx, const MerkleCertificate* cert);

	void validate(const BlockMerkleRoot* merkleRoot) const;

	const AbstractBlockchainTransaction* getTransaction() const noexcept {
		return this->trx;
	}

private:
	AbstractBlockchainTransaction* trx;
	MerkleCertificate* cert;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_BLOCKTRANSACTIONTRANSFERDATA_H_ */
