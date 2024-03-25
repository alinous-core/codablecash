/*
 * VoterEntryFactory.h
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_VOTERENTRYFACTORY_H_
#define BC_FINALIZER_VOTERENTRYFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class VoterEntryFactory : public AbstractBtreeDataFactory {
public:
	VoterEntryFactory();
	virtual ~VoterEntryFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_VOTERENTRYFACTORY_H_ */
