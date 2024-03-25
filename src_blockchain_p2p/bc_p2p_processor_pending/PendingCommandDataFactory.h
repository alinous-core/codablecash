/*
 * AbstructNodeQueueCommandDataFactory.h
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATAFACTORY_H_
#define BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class PendingCommandDataFactory : public AbstractBtreeDataFactory {
public:
	PendingCommandDataFactory();
	virtual ~PendingCommandDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATAFACTORY_H_ */
