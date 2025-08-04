/*
 * ClientSyncHeaderCommandResponse.h
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/ArrayList.h"

#include "bc_status_cache/BlockchainController.h"


namespace codablecash {

class ClientSyncHeaderHeightData;

class ClientSyncHeaderCommandResponse : public AbstractCommandResponse, public IBlockDetectCallback{
public:
	ClientSyncHeaderCommandResponse();
	virtual ~ClientSyncHeaderCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	virtual void detect(uint16_t height, const ArrayList<Block>* list);

	int getDataCount() const noexcept;
	const ClientSyncHeaderHeightData* get(int i) const noexcept;

protected:
	ArrayList<ClientSyncHeaderHeightData>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMANDRESPONSE_H_ */
