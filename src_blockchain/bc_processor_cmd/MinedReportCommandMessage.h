/*
 * MinedReportCommandMessage.h
 *
 *  Created on: 2023/04/04
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_MINEDREPORTCOMMANDMESSAGE_H_
#define BC_BLOCK_GENERATOR_MINEDREPORTCOMMANDMESSAGE_H_

#include "bc_processor/AbstractCentralProcessorCommandMessage.h"

namespace codablecash {

class Block;

class MinedReportCommandMessage : public AbstractCentralProcessorCommandMessage {
public:
	explicit MinedReportCommandMessage(const Block* block);
	virtual ~MinedReportCommandMessage();

protected:
	virtual void process(CentralProcessor* processor);
private:
	Block* block;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_MINEDREPORTCOMMANDMESSAGE_H_ */
