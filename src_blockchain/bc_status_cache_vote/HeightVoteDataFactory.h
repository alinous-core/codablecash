/*
 * HeightVoteDataFactory.h
 *
 *  Created on: 2024/07/23
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_VOTE_HEIGHTVOTEDATAFACTORY_H_
#define BC_STATUS_CACHE_VOTE_HEIGHTVOTEDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class HeightVoteDataFactory : public AbstractBtreeDataFactory {
public:
	HeightVoteDataFactory();
	virtual ~HeightVoteDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_VOTE_HEIGHTVOTEDATAFACTORY_H_ */
