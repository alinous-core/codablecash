/*
 * AbstractPoWCommandMessage.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef POW_ABSTRACTPOWCOMMANDMESSAGE_H_
#define POW_ABSTRACTPOWCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class PoWManager;


class AbstractPoWCommandMessage : public ICommandMessage {
public:
	AbstractPoWCommandMessage();
	virtual ~AbstractPoWCommandMessage();

	virtual void execute(ICommandParameter* param);

protected:
	virtual void process(PoWManager* manager) = 0;
};

} /* namespace codablecash */

#endif /* POW_ABSTRACTPOWCOMMANDMESSAGE_H_ */
