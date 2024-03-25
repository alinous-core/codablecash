/*
 * AbstractNetworkTransferMessageCommad.h
 *
 *  Created on: 2024/02/25
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_ABSTRACTNETWORKTRANSFERCOMMANDMESSAGE_H_
#define BC_P2P_PROCESSOR_ABSTRACTNETWORKTRANSFERCOMMANDMESSAGE_H_

#include "procesor/ICommandMessage.h"

namespace codablecash {

class AbstractNetworkTransferCommandMessage : public ICommandMessage {
public:
	AbstractNetworkTransferCommandMessage();
	virtual ~AbstractNetworkTransferCommandMessage();
};

}  // namespace codablecash

#endif /* BC_P2P_PROCESSOR_ABSTRACTNETWORKTRANSFERCOMMANDMESSAGE_H_ */
