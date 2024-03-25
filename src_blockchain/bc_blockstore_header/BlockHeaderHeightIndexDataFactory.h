/*
 * BlockHeaderHeightIndexDataFactory.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXDATAFACTORY_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderHeightIndexDataFactory : public AbstractBtreeDataFactory {
public:
	BlockHeaderHeightIndexDataFactory();
	virtual ~BlockHeaderHeightIndexDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXDATAFACTORY_H_ */
