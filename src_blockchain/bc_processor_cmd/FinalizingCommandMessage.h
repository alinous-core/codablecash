/*
 * FinalizingCommandMessage.h
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#ifndef BC_PROCESSOR_CMD_FINALIZINGCOMMANDMESSAGE_H_
#define BC_PROCESSOR_CMD_FINALIZINGCOMMANDMESSAGE_H_

#include <cstdint>

#include "bc_processor/AbstractCentralProcessorCommandMessage.h"

namespace codablecash {

class BlockHeaderId;

class FinalizingCommandMessage  : public AbstractCentralProcessorCommandMessage {
public:
	FinalizingCommandMessage(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId);
	virtual ~FinalizingCommandMessage();

protected:
	virtual void process(CentralProcessor* processor);

private:
	uint16_t zone;
	uint64_t finalizingHeight;
	BlockHeaderId *headerId;
};

} /* namespace codablecash */

#endif /* BC_PROCESSOR_CMD_FINALIZINGCOMMANDMESSAGE_H_ */
