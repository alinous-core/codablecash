/*
 * TransactionIdListData.h
 *
 *  Created on: Jun 9, 2026
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_WALLETTRANSACTIONIDLISTDATA_H_
#define BC_WALLET_NET_DATA_WALLETTRANSACTIONIDLISTDATA_H_

#include "filestore_block/IBlockObject.h"
#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class TransactionId;

class WalletTransactionIdListData : public alinous::IBlockObject {
public:
	WalletTransactionIdListData();
	virtual ~WalletTransactionIdListData();

	void join(const WalletTransactionIdListData* value) noexcept;
	bool contains(const TransactionId* value) const noexcept;
	void remove(const TransactionId* value) noexcept;
	int indexof(const TransactionId* value) const noexcept;
	bool isEmpty() const noexcept;

	virtual IBlockObject* copyData() const noexcept;

	void add(const TransactionId* trxId);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static WalletTransactionIdListData* fromBinary(ByteBuffer* in);

	const TransactionId* next() noexcept{
		if(this->index == this->list->size()){
			return nullptr;
		}
		return this->list->get(this->index++);
	}

private:
	ArrayList<TransactionId>* list;
	int index;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_WALLETTRANSACTIONIDLISTDATA_H_ */
