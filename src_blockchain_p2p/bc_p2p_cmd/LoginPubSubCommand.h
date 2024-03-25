/*
 * LoginCommand.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_LOGINPUBSUBCOMMAND_H_
#define BC_P2P_CMD_LOGINPUBSUBCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainLoginCommand.h"

namespace codablecash {

class LoginPubSubCommand : public AbstractBlockchainLoginCommand {
public:
	LoginPubSubCommand(const LoginPubSubCommand& inst);
	LoginPubSubCommand(uint16_t zone);
	virtual ~LoginPubSubCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;

	virtual IBlockObject* copyData() const noexcept;

protected:
	virtual ByteBuffer* getSignBinary() const;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_LOGINPUBSUBCOMMAND_H_ */
