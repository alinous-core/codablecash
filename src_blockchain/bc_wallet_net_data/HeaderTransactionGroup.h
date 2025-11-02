/*
 * HeaderTransactionGroup.h
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUP_H_
#define BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUP_H_

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderId;
class AbstractBlockchainTransaction;
class TransactionId;

class HeaderTransactionGroup : public alinous::IBlockObject {
public:
	HeaderTransactionGroup(const HeaderTransactionGroup& inst);
	HeaderTransactionGroup();
	virtual ~HeaderTransactionGroup();


	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static HeaderTransactionGroup* createFromBinary(ByteBuffer* i);

	virtual IBlockObject* copyData() const noexcept;

	void setHeaderId(const BlockHeaderId* headerId);
	void addTransaction(const AbstractBlockchainTransaction* trx);

	void join(const HeaderTransactionGroup* value) noexcept;
	bool contains(const AbstractBlockchainTransaction* trx) const noexcept;
	void remove(const TransactionId* trxId) noexcept;
	int indexof(const TransactionId* trxId) const noexcept;

	const ArrayList<AbstractBlockchainTransaction>* getTransactionsList() const noexcept {
		return this->list;
	}

private:
	BlockHeaderId* headerId;
	ArrayList<AbstractBlockchainTransaction>* list;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUP_H_ */
