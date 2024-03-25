/*
 * SyncHeaderDataFactory.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERDATAFACTORY_H_
#define BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class SyncHeaderDataFactory : public AbstractBtreeDataFactory {
public:
	SyncHeaderDataFactory();
	virtual ~SyncHeaderDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_REPO_SYNCHEADERDATAFACTORY_H_ */
