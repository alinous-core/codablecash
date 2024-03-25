/*
 * TransactionIdKey.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_TRANSACTIONIDKEY_H_
#define BC_WALLET_TRX_REPO_TRANSACTIONIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class TransactionId;

class TransactionIdKey : public AbstractBtreeKey {
public:
	TransactionIdKey();
	explicit TransactionIdKey(const TransactionId* trxId);
	virtual ~TransactionIdKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransactionIdKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

//	const TransactionId* getTransactionId() const noexcept {
//		return this->trxId;
//	}

private:
	TransactionId* trxId;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_TRANSACTIONIDKEY_H_ */
