/*
 * DownloadTransactionsNodeCommandResponse.h
 *
 *  Created on: 2023/11/27
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AbstractBlockchainTransaction;

class DownloadTransactionsNodeCommandResponse : public AbstractCommandResponse {
public:
	DownloadTransactionsNodeCommandResponse();
	virtual ~DownloadTransactionsNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void addTransaction(const AbstractBlockchainTransaction* trx);

	int size() const noexcept;
	AbstractBlockchainTransaction* get(int i) const noexcept;


private:
	ArrayList<AbstractBlockchainTransaction>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONSNODECOMMANDRESPONSE_H_ */
