/*
 * AbstractNetworkInfoCommandMessage.h
 *
 *  Created on: Feb 2, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_ABSTRACTNETWORKINFOCOMMANDMESSAGE_H_
#define BC_P2P_PROCESSOR_ABSTRACTNETWORKINFOCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class AbstractNetworkInfoCommandMessage : public ICommandMessage {
public:
	AbstractNetworkInfoCommandMessage();
	virtual ~AbstractNetworkInfoCommandMessage();
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_ABSTRACTNETWORKINFOCOMMANDMESSAGE_H_ */
