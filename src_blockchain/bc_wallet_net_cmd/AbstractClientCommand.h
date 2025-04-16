/*
 * AbstractClientCommand.h
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_ABSTRACTCLIENTCOMMAND_H_
#define BC_WALLET_NET_CMD_ABSTRACTCLIENTCOMMAND_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class NetworkWallet;

class AbstractClientCommand : public IBlockObject {
public:
	static const constexpr uint8_t CLIENT_NEW_TRANSACTION{1};

	AbstractClientCommand(const AbstractClientCommand& inst);
	explicit AbstractClientCommand(uint8_t type);
	virtual ~AbstractClientCommand();

	static AbstractClientCommand* createFromBinary(ByteBuffer* in);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual void process(NetworkWallet* wallet) const = 0;

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_ABSTRACTCLIENTCOMMAND_H_ */
