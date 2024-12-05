/*
 * LockInOperationDataFactory.h
 *
 *  Created on: Oct 18, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONDATAFACTORY_H_
#define BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class LockInOperationDataFactory: public alinous::AbstractBtreeDataFactory {
public:
	LockInOperationDataFactory();
	virtual ~LockInOperationDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_LOCKINOPERATIONDATAFACTORY_H_ */
