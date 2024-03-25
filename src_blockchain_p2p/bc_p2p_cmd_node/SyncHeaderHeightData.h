/*
 * SyncHeaderHeightData.h
 *
 *  Created on: 2023/11/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_SYNCHEADERHEIGHTDATA_H_
#define BC_P2P_CMD_NODE_SYNCHEADERHEIGHTDATA_H_

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderTransferData;

class SyncHeaderHeightData : public IBlockObject {
public:
	SyncHeaderHeightData(const SyncHeaderHeightData& inst);
	SyncHeaderHeightData();
	virtual ~SyncHeaderHeightData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static SyncHeaderHeightData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	UnicodeString* toString() const noexcept;

	uint64_t getHeight() const noexcept {
		return this->height;
	}
	void setHeight(uint64_t height) noexcept;

	void addHeaderData(const BlockHeaderTransferData* data);

	const ArrayList<BlockHeaderTransferData>* getDataList() const noexcept {
		return this->list;
	}

	void verifyVoteTransactionsData() const;

private:
	uint64_t height;
	ArrayList<BlockHeaderTransferData>* list;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_SYNCHEADERHEIGHTDATA_H_ */
