/*
 * HeightHeaderSyncDataList.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_REPO_HEIGHTHEADERSYNCDATALIST_H_
#define BC_NETWORK_INSTANCE_SYNC_REPO_HEIGHTHEADERSYNCDATALIST_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderTransferData;
class TransferedDataId;

class HeightHeaderSyncDataList : public IBlockObject {
public:
	HeightHeaderSyncDataList(const HeightHeaderSyncDataList& inst);
	HeightHeaderSyncDataList();
	virtual ~HeightHeaderSyncDataList();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;

	static HeightHeaderSyncDataList* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void join(const HeightHeaderSyncDataList* newValue);

	bool hasId(const TransferedDataId* dataId) const noexcept;
	void addData(const BlockHeaderTransferData* data) noexcept;

	const ArrayList<BlockHeaderTransferData>* getList() const noexcept {
		return this->list;
	}

private:
	ArrayList<BlockHeaderTransferData>* list;

};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_REPO_HEIGHTHEADERSYNCDATALIST_H_ */
