/*
 * DownloadDnsInfoCommandResponse.cpp
 *
 *  Created on: 2023/10/16
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/DownloadDnsInfoCommandResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

#include "bc_p2p_info/P2pNodeRecord.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"
namespace codablecash {

DownloadDnsInfoCommandResponse::DownloadDnsInfoCommandResponse() : AbstractCommandResponse(TYPE_RES_DOWNLOAD_DNS_INFO) {
	this->list = new ArrayList<P2pNodeRecord>();
}

DownloadDnsInfoCommandResponse::~DownloadDnsInfoCommandResponse() {
	this->list->deleteElements();
	delete this->list;
}

int DownloadDnsInfoCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* record = this->list->get(i);

		total += record->binarySize();
	}

	return total;
}

void DownloadDnsInfoCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	int maxLoop = this->list->size();
	BinaryUtils::checkUShortRange(maxLoop, 0, DownloadDnsInfoCommand::MAX_DL);
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* record = this->list->get(i);

		record->toBinary(buff);
	}
}

void DownloadDnsInfoCommandResponse::fromBinary(ByteBuffer *buff) {
	int maxLoop = buff->getShort();
	BinaryUtils::checkUShortRange(maxLoop, 0, DownloadDnsInfoCommand::MAX_DL);

	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* record = P2pNodeRecord::fromBinary(buff);

		this->list->addElement(record);
	}
}

UnicodeString* DownloadDnsInfoCommandResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"");

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* record = this->list->get(i);

		UnicodeString* recordStr = record->toString(); __STP(recordStr);
		if(i != 0){
			str->append(L", ");
		}
		str->append(recordStr);
	}

	return str;
}

void DownloadDnsInfoCommandResponse::importRecords(const ArrayList<P2pNodeRecord> *records) {
	int maxLoop = records->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecord* record = records->get(i);

		this->list->addElement(new P2pNodeRecord(*record));
	}
}

} /* namespace codablecash */
