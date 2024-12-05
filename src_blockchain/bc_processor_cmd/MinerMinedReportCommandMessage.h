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

class MinerMinedReportCommandMessage : public AbstractCentralProcessorCommandMessage {
public:
	explicit MinerMinedReportCommandMessage(const Block* block);
	virtual ~MinerMinedReportCommandMessage();

protected:
	virtual void process(CentralProcessor* processor);
private:
	Block* block;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_MINEDREPORTCOMMANDMESSAGE_H_ */
