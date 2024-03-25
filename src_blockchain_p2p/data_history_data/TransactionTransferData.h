/*
 * TransactionTransferData.h
 *
 *  Created on: 2023/09/24
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_TRANSACTIONTRANSFERDATA_H_
#define DATA_HISTORY_DATA_TRANSACTIONTRANSFERDATA_H_

#include "data_history/AbstractTransferedData.h"

namespace codablecash {

class AbstractBlockchainTransaction;

class TransactionTransferData : public AbstractTransferedData {
public:
	TransactionTransferData(const TransactionTransferData& inst);
	TransactionTransferData();
	virtual ~TransactionTransferData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual TransferedDataId* getTransferedDataId() const;

	void setTransaction(const AbstractBlockchainTransaction* trx) noexcept;

	AbstractBlockchainTransaction* getTransaction() const noexcept {
		return this->trx;
	}

private:
	AbstractBlockchainTransaction* trx;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_TRANSACTIONTRANSFERDATA_H_ */
