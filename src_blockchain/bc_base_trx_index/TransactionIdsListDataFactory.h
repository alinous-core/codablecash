/*
 * TransactionIdsListDataFactory.h
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATAFACTORY_H_
#define BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class TransactionIdsListDataFactory : public AbstractBtreeDataFactory {
public:
	TransactionIdsListDataFactory();
	virtual ~TransactionIdsListDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_TRANSACTIONIDSLISTDATAFACTORY_H_ */
