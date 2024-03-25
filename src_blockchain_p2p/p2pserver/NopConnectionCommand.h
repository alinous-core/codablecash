/*
 * NopConnectionCommand.h
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#ifndef P2PSERVER_NOPCONNECTIONCOMMAND_H_
#define P2PSERVER_NOPCONNECTIONCOMMAND_H_

#include "p2pserver/AbstractConnectionCommand.h"

namespace codablecash {

class NopConnectionCommand : public AbstractConnectionCommand {
public:
	NopConnectionCommand();
	virtual ~NopConnectionCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize();
	virtual void toBinary(ByteBuffer* buff);

	virtual void process(P2pConnectionAcceptThread* acceptThread);
};

} /* namespace codablecash */

#endif /* P2PSERVER_NOPCONNECTIONCOMMAND_H_ */
