/*
 * TransactionIdDataFactory.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_TRANSACTIONIDDATAFACTORY_H_
#define BC_BASE_TRX_INDEX_TRANSACTIONIDDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class TransactionIdDataFactory : public AbstractBtreeDataFactory {
public:
	TransactionIdDataFactory();
	virtual ~TransactionIdDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_TRANSACTIONIDDATAFACTORY_H_ */
