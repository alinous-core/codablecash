/*
 * DownloadDnsInfoCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"
#include "bc_p2p_cmd_client/DownloadDnsInfoCommandResponse.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_p2p_info/P2pDnsManager.h"
#include "bc_p2p_info/P2pNodeRecord.h"

namespace codablecash {

DownloadDnsInfoCommand::DownloadDnsInfoCommand(const DownloadDnsInfoCommand &inst)
		: AbstractClientRequestCommand(inst) {
	this->zone = inst.zone;
	this->maxNum = inst.maxNum;
}

DownloadDnsInfoCommand::DownloadDnsInfoCommand(uint16_t zone) : AbstractClientRequestCommand(TYPE_CLIENT_DOWNLOAD_DNS) {
	this->zone = zone;
	this->maxNum = 256;
}

DownloadDnsInfoCommand::~DownloadDnsInfoCommand() {

}

AbstractCommandResponse* DownloadDnsInfoCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	P2pDnsManager* p2pManager = inst->getP2pDnsManager();

	DownloadDnsInfoCommandResponse* respose = new DownloadDnsInfoCommandResponse(); __STP(respose);

	ArrayList<P2pNodeRecord>* list = p2pManager->getZoneRecords(this->zone, this->maxNum); __STP(list);
	list->setDeleteOnExit();

	respose->importRecords(list);

	return __STP_MV(respose);
}

IBlockObject* DownloadDnsInfoCommand::copyData() const noexcept {
	return new DownloadDnsInfoCommand(*this);
}

int DownloadDnsInfoCommand::binarySize() const {
	BinaryUtils::checkUShortRange(this->maxNum, 1, MAX_DL);

	int total = AbstractClientRequestCommand::binarySize();
	total += sizeof(this->zone);
	total += sizeof(this->maxNum);

	return total;
}

void DownloadDnsInfoCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkUShortRange(this->maxNum, 1, 1024);

	AbstractClientRequestCommand::toBinary(buff);

	buff->putShort(this->zone);
	buff->putShort(this->maxNum);
}

ByteBuffer* DownloadDnsInfoCommand::getSignBinary() const {
	BinaryUtils::checkUShortRange(this->maxNum, 1, 1024);

	int total = sizeof(this->zone) + sizeof(this->maxNum);
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);

	buff->putShort(this->zone);
	buff->putShort(this->maxNum);

	return buff;
}

void DownloadDnsInfoCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	this->zone = buff->getShort();
	this->maxNum = buff->getShort();

	BinaryUtils::checkUShortRange(this->maxNum, 1, 1024);
}

} /* namespace codablecash */
