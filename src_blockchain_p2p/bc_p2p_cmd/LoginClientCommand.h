/*
 * LoginClientCommand.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_LOGINCLIENTCOMMAND_H_
#define BC_P2P_CMD_LOGINCLIENTCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainLoginCommand.h"

namespace codablecash {

class LoginClientCommand : public AbstractBlockchainLoginCommand {
public:
	LoginClientCommand(const LoginClientCommand& inst);
	explicit LoginClientCommand(uint16_t zone);
	virtual ~LoginClientCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;

	virtual IBlockObject* copyData() const noexcept;

protected:
	virtual ByteBuffer* getSignBinary() const;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_LOGINCLIENTCOMMAND_H_ */
