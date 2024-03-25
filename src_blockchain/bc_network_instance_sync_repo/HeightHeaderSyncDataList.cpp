/*
 * HeightHeaderSyncDataList.cpp
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#include "bc_network_instance_sync_repo/HeightHeaderSyncDataList.h"

#include "data_history/TransferedDataId.h"

#include "data_history_data/BlockHeaderTransferData.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

HeightHeaderSyncDataList::HeightHeaderSyncDataList(const HeightHeaderSyncDataList &inst) {
	this->list = new ArrayList<BlockHeaderTransferData>();
}

HeightHeaderSyncDataList::HeightHeaderSyncDataList() {
	this->list = new ArrayList<BlockHeaderTransferData>();
}

HeightHeaderSyncDataList::~HeightHeaderSyncDataList() {
	this->list->deleteElements();
	delete this->list;
}

int HeightHeaderSyncDataList::binarySize() const {
	int total = sizeof(uint32_t);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeaderTransferData* data = this->list->get(i);
		total += data->binarySize();
	}

	return total;
}

void HeightHeaderSyncDataList::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list->size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		BlockHeaderTransferData* data = this->list->get(i);
		data->toBinary(out);
	}
}

HeightHeaderSyncDataList* HeightHeaderSyncDataList::createFromBinary(ByteBuffer *in) {
	HeightHeaderSyncDataList* inst = new HeightHeaderSyncDataList();

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		AbstractTransferedData* data = AbstractTransferedData::createFromBinary(in); __STP(data);
		BlockHeaderTransferData* headerData = dynamic_cast<BlockHeaderTransferData*>(data);

		BinaryUtils::checkNotNull(headerData);

		__STP_MV(data);
		inst->list->addElement(headerData);
	}

	return inst;
}

IBlockObject* HeightHeaderSyncDataList::copyData() const noexcept {
	return new HeightHeaderSyncDataList(*this);
}

void HeightHeaderSyncDataList::join(const HeightHeaderSyncDataList *newValue) {
	int maxLoop = newValue->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const BlockHeaderTransferData* d = newValue->list->get(i);

		TransferedDataId* dataId = d->getTransferedDataId(); __STP(dataId);
		if(!hasId(dataId)){
			addData(d);
		}
	}
}

bool HeightHeaderSyncDataList::hasId(const TransferedDataId *dataId) const noexcept {
	bool ret = false;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		BlockHeaderTransferData* data = this->list->get(i);
		TransferedDataId* id = data->getTransferedDataId(); __STP(id);

		if(dataId->equals(id)){
			ret = true;
			break;
		}
	}

	return ret;
}

void HeightHeaderSyncDataList::addData(const BlockHeaderTransferData *data) noexcept {
	this->list->addElement(dynamic_cast<BlockHeaderTransferData*>(data->copyData()));
}

} /* namespace codablecash */
