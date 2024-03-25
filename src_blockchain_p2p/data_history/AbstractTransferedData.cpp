/*
 * ITransferedData.cpp
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#include "data_history/AbstractTransferedData.h"

#include "data_history_data/TransactionTransferData.h"
#include "data_history_data/BlockHeaderTransferData.h"


namespace codablecash {

AbstractTransferedData::AbstractTransferedData(const AbstractTransferedData &inst) {
	this->type = inst.type;
}

AbstractTransferedData::AbstractTransferedData(uint8_t type) {
	this->type = type;
}

AbstractTransferedData::~AbstractTransferedData() {

}

AbstractTransferedData* AbstractTransferedData::createFromBinary(ByteBuffer *in) {
	AbstractTransferedData* ret = nullptr;

	uint8_t type = in->get();

	switch(type){
	case DATA_TRANSACTION:
		ret = new TransactionTransferData();
		break;
	case DATA_BLOCKHEADER:
		ret = new BlockHeaderTransferData();
		break;
	default:
		return nullptr;
	}

	ret->fromBinary(in);

	return ret;
}

} /* namespace codablecash */
