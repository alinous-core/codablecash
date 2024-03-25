/*
 * SyncMempoolNodeCommandResponse.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_memorypool/MemPoolTransaction.h"

#include "base/ArrayList.h"


namespace codablecash {

class AbstractBlockchainTransaction;

class SyncMempoolNodeCommandResponse : public AbstractCommandResponse, public ITransactionListCallback {
public:
	SyncMempoolNodeCommandResponse();
	virtual ~SyncMempoolNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	virtual void addBlockchainTransaction(const AbstractBlockchainTransaction* trx) noexcept;

	int getResultCount() const noexcept;
	AbstractBlockchainTransaction* get(int i) const noexcept;

private:
	ArrayList<AbstractBlockchainTransaction>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SYNCMEMPOOLNODECOMMANDRESPONSE_H_ */
