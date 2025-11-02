/*
 * AbstractClientCommand.h
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_ABSTRACTCLIENTQUEUECOMMAND_H_
#define BC_WALLET_NET_CMD_ABSTRACTCLIENTQUEUECOMMAND_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class NetworkWallet;

class AbstractClientQueueCommand : public IBlockObject {
public:
	static const constexpr uint8_t CLIENT_NEW_TRANSACTION{1};
	static const constexpr uint8_t CLIENT_BLOCK_MINED{2};

	AbstractClientQueueCommand(const AbstractClientQueueCommand& inst);
	explicit AbstractClientQueueCommand(uint8_t type);
	virtual ~AbstractClientQueueCommand();

	static AbstractClientQueueCommand* createFromBinary(ByteBuffer* in);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual void process(NetworkWallet* wallet) const = 0;

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_ABSTRACTCLIENTQUEUECOMMAND_H_ */
