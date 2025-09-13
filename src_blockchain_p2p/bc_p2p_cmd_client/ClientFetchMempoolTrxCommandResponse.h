/*
 * ClientFetchMempoolTrxCommandResponse.h
 *
 *  Created on: Aug 17, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/ArrayList.h"

namespace codablecash {

class AbstractBlockchainTransaction;

class ClientFetchMempoolTrxCommandResponse : public AbstractCommandResponse {
public:
	ClientFetchMempoolTrxCommandResponse();
	virtual ~ClientFetchMempoolTrxCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual UnicodeString* toString() const noexcept;

	void addTransaction(const AbstractBlockchainTransaction* trx);

	const ArrayList<AbstractBlockchainTransaction>* getList() const noexcept {
		return this->list;
	}

private:
	ArrayList<AbstractBlockchainTransaction>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTFETCHMEMPOOLTRXCOMMANDRESPONSE_H_ */
