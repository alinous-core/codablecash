/*
 * NopCommandMessage.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef PROCESOR_NOPCOMMANDMESSAGE_H_
#define PROCESOR_NOPCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class NopCommandMessage: public ICommandMessage {
public:
	NopCommandMessage();
	virtual ~NopCommandMessage();

	virtual void execute(ICommandParameter* param);
};

} /* namespace codablecash */

#endif /* PROCESOR_NOPCOMMANDMESSAGE_H_ */
