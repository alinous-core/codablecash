/*
 * TransactionData.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_TRANSACTIONDATA_H_
#define BC_WALLET_TRX_REPO_TRANSACTIONDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;

class TransactionData : public IBlockObject {
public:
	explicit TransactionData(const AbstractBlockchainTransaction* trx);
	virtual ~TransactionData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransactionData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const AbstractBlockchainTransaction* getTrx() const noexcept {
		return this->trx;
	}

private:
	AbstractBlockchainTransaction* trx;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_TRANSACTIONDATA_H_ */
