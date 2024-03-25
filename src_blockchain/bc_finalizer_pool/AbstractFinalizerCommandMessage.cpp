/*
 * AbstractFinalizerCommandMessage.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "bc_finalizer_pool/AbstractFinalizerCommandMessage.h"

#include "bc_finalizer_pool/FinalizerPool.h"

namespace codablecash {

AbstractFinalizerCommandMessage::AbstractFinalizerCommandMessage() {

}

AbstractFinalizerCommandMessage::~AbstractFinalizerCommandMessage() {

}

void AbstractFinalizerCommandMessage::execute(ICommandParameter *param) {
	FinalizerPool* pool = dynamic_cast<FinalizerPool*>(param);

	process(pool);
}

} /* namespace codablecash */
