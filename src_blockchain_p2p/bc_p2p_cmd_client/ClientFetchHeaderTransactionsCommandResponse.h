/*
 * ClientFetchHeaderTransactionsCommandResponse.h
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

namespace codablecash {

class ClientBlockHeaderTransferData;


class ClientFetchHeaderTransactionsCommandResponse : public AbstractCommandResponse {
public:
	ClientFetchHeaderTransactionsCommandResponse();
	virtual ~ClientFetchHeaderTransactionsCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual UnicodeString* toString() const noexcept;

	void setTransferData(ClientBlockHeaderTransferData* transferData);
	const ClientBlockHeaderTransferData* getTransferData() const noexcept {
		return this->transferData;
	}

private:
	ClientBlockHeaderTransferData* transferData;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTFETCHHEADERTRANSACTIONSCOMMANDRESPONSE_H_ */
