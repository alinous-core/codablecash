/*
 * ClientFetchHeaderTransactionsCommand.h
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMAND_H_
#define BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

namespace codablecash {

class BlockHeaderId;

class ClientFetchHeaderTransactionsCommand : public AbstractClientRequestCommand {
public:
	ClientFetchHeaderTransactionsCommand(const ClientFetchHeaderTransactionsCommand& inst);
	ClientFetchHeaderTransactionsCommand();
	virtual ~ClientFetchHeaderTransactionsCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	void setZone(uint16_t z) noexcept {
		this->zone = z;
	}
	void setBlockHeaderId(const BlockHeaderId* headerId);
	void setHeight(uint64_t height) noexcept {
		this->height = height;
	}

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;

private:
	uint16_t zone;
	BlockHeaderId* headerId;
	uint64_t height;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMAND_H_ */
