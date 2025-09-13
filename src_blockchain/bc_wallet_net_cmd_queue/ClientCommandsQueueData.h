/*
 * ClientCommandsQueueData.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATA_H_
#define BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AbstractTransferedData;
class AbstractClientQueueCommand;
class NodeIdentifier;

class ClientCommandsQueueData : public IBlockObject {
public:
	ClientCommandsQueueData(const ClientCommandsQueueData& inst);
	ClientCommandsQueueData();
	virtual ~ClientCommandsQueueData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static ClientCommandsQueueData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setData(const AbstractTransferedData* dt);
	void setSourceNodeId(const NodeIdentifier* nodeId);

	AbstractClientQueueCommand* toClientCommand() const;

private:
	AbstractTransferedData* data;
	NodeIdentifier* sourceNodeId;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUEDATA_H_ */
