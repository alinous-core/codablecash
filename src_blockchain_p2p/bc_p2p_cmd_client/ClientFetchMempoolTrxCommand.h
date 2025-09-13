/*
 * ClientFetchMempoolTrx.h
 *
 *  Created on: Aug 15, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMAND_H_
#define BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

namespace codablecash {

class ClientFetchMempoolTrxCommand : public AbstractClientRequestCommand {
public:
	ClientFetchMempoolTrxCommand(const ClientFetchMempoolTrxCommand& inst);
	ClientFetchMempoolTrxCommand();
	virtual ~ClientFetchMempoolTrxCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	void setZone(uint16_t zone) noexcept;

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;

private:
	uint16_t zone;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMAND_H_ */
