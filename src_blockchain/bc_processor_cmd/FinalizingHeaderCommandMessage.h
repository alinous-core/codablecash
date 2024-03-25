/*
 * FinalizingHeaderCommandMessage.h
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#ifndef BC_PROCESSOR_CMD_FINALIZINGHEADERCOMMANDMESSAGE_H_
#define BC_PROCESSOR_CMD_FINALIZINGHEADERCOMMANDMESSAGE_H_

#include "bc_processor/AbstractCentralProcessorCommandMessage.h"

#include <cstdint>

namespace codablecash {

class BlockHeaderId;

class FinalizingHeaderCommandMessage : public AbstractCentralProcessorCommandMessage {
public:
	FinalizingHeaderCommandMessage(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId *headerId);
	virtual ~FinalizingHeaderCommandMessage();

protected:
	virtual void process(CentralProcessor* processor);

private:
	uint16_t zone;
	uint64_t finalizingHeight;
	BlockHeaderId *headerId;
};

} /* namespace codablecash */

#endif /* BC_PROCESSOR_CMD_FINALIZINGHEADERCOMMANDMESSAGE_H_ */
