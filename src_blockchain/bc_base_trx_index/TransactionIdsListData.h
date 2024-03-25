/*
 * TransactionIdsListData.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATA_H_
#define BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATA_H_

#include "filestore_block/IBlockObject.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class TransactionId;

class TransactionIdsListData : public alinous::IBlockObject {
public:
	TransactionIdsListData();
	virtual ~TransactionIdsListData();

	void join(const TransactionIdsListData* value) noexcept;
	bool contains(const TransactionId* value) const noexcept;
	void remove(const TransactionId* value) noexcept;
	int indexof(const TransactionId* value) const noexcept;
	bool isEmpty() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransactionIdsListData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void add(const TransactionId* trxId);

	const TransactionId* next() noexcept{
		if(this->index == this->list.size()){
			return nullptr;
		}
		return this->list.get(this->index++);
	}

	const ArrayList<TransactionId>* getList() const noexcept {
		return &list;
	}

private:
	ArrayList<TransactionId> list;
	int index;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATA_H_ */
