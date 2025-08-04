/*
 * SyncHeaderNodeCommandResponse.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SYNCHEADERNODECOMMANDRESPONSE_H_
#define BC_P2P_CMD_NODE_SYNCHEADERNODECOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_status_cache/BlockchainController.h"

#include "base/ArrayList.h"

namespace codablecash {

class SyncHeaderHeightData;

class SyncHeaderNodeCommandResponse : public AbstractCommandResponse, public IBlockDetectCallback {
public:
	SyncHeaderNodeCommandResponse();
	virtual ~SyncHeaderNodeCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	int getDataCount() const noexcept;
	const SyncHeaderHeightData* get(int i) const noexcept;

	virtual void detect(uint16_t height, const ArrayList<Block>* list);

protected:
	ArrayList<SyncHeaderHeightData>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SYNCHEADERNODECOMMANDRESPONSE_H_ */
