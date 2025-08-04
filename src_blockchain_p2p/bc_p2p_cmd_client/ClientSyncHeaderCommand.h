/*
 * ClientSyncHeaderNodeCommand.h
 *
 *  Created on: Apr 22, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMAND_H_
#define BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class ClientSyncHeaderHeightData;
class CodablecashNodeInstance;
class BloomFilter1024;
class Block;
class ClientBlockHeaderTransferData;

class ClientSyncHeaderCommand : public AbstractClientRequestCommand {
public:
	ClientSyncHeaderCommand(const ClientSyncHeaderCommand& inst);
	ClientSyncHeaderCommand();
	virtual ~ClientSyncHeaderCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	virtual ByteBuffer* getSignBinary() const;
	virtual IBlockObject* copyData() const noexcept;

	void setZone(uint16_t zone) noexcept;
	void setNumRequestHeight(uint32_t numRequestHeight) noexcept;
	void setStartHeight(uint32_t offset) noexcept;

	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;


private:
	void handleBlockHeightTransactions(const ClientSyncHeaderHeightData* data, const ArrayList<BloomFilter1024>* filtersList, CodablecashNodeInstance *inst) const;
	void handleBlockTransactions(ClientBlockHeaderTransferData* data, const Block* block, const ArrayList<BloomFilter1024>* filtersList, CodablecashNodeInstance *inst) const;

private:
	uint16_t zone;
	uint32_t numRequestHeight;
	uint64_t startHeight;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERCOMMAND_H_ */
