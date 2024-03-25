/*
 * AbstractFinalizerCommandMessage.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_POOL_ABSTRACTFINALIZERCOMMANDMESSAGE_H_
#define BC_FINALIZER_POOL_ABSTRACTFINALIZERCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class FinalizerPool;

class AbstractFinalizerCommandMessage : public ICommandMessage {
public:
	AbstractFinalizerCommandMessage();
	virtual ~AbstractFinalizerCommandMessage();

	virtual void execute(ICommandParameter* param);

protected:
	virtual void process(FinalizerPool* pool) = 0;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_POOL_ABSTRACTFINALIZERCOMMANDMESSAGE_H_ */
