/*
 * TransferedDataIdListFactory.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAIDLISTFACTORY_H_
#define DATA_HISTORY_TRANSFEREDDATAIDLISTFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class TransferedDataIdListFactory  : public AbstractBtreeDataFactory {
public:
	TransferedDataIdListFactory();
	virtual ~TransferedDataIdListFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAIDLISTFACTORY_H_ */
