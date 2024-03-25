/*
 * DummyCommand.h
 *
 *  Created on: 2023/08/15
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_FW_DUMMYERRORNOPCOMMAND_H_
#define BLOCKCHAIN_FW_DUMMYERRORNOPCOMMAND_H_

#include "procesor/NopCommandMessage.h"

namespace codablecash {

class DummyErrorNopCommand : public NopCommandMessage {
public:
	DummyErrorNopCommand();
	virtual ~DummyErrorNopCommand();

	virtual void execute(ICommandParameter* param);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_FW_DUMMYERRORNOPCOMMAND_H_ */
