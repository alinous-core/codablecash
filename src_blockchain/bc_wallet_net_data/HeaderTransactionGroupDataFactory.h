/*
 * HeaderTransactionGroupDataFactory.h
 *
 *  Created on: Jul 4, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUPDATAFACTORY_H_
#define BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUPDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class HeaderTransactionGroupDataFactory : public AbstractBtreeDataFactory {
public:
	HeaderTransactionGroupDataFactory();
	virtual ~HeaderTransactionGroupDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_HEADERTRANSACTIONGROUPDATAFACTORY_H_ */
