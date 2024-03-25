/*
 * AbstractCentralProcessorCommandMessage.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef BC_PROCESSOR_ABSTRACTCENTRALPROCESSORCOMMANDMESSAGE_H_
#define BC_PROCESSOR_ABSTRACTCENTRALPROCESSORCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class CentralProcessor;

class AbstractCentralProcessorCommandMessage : public ICommandMessage {
public:
	AbstractCentralProcessorCommandMessage();
	virtual ~AbstractCentralProcessorCommandMessage();

	virtual void execute(ICommandParameter* param);

protected:
	virtual void process(CentralProcessor* param) = 0;

protected:
	CentralProcessor* processor;
};

} /* namespace codablecash */

#endif /* BC_PROCESSOR_ABSTRACTCENTRALPROCESSORCOMMANDMESSAGE_H_ */
