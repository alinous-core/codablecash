/*
 * DownloadOmittedBlockBodyNodeCommandResponse.cpp
 *
 *  Created on: 2023/11/24
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommandResponse.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_block_body/OmittedBlockBody.h"

#include "bc_block/BlockMerkleRoot.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

DownloadOmittedBlockBodyNodeCommandResponse::DownloadOmittedBlockBodyNodeCommandResponse()
		:AbstractCommandResponse(TYPE_RES_DOWNLOAD_OMITTED_BLOCK_BODY) {
	this->body = nullptr;
}

DownloadOmittedBlockBodyNodeCommandResponse::~DownloadOmittedBlockBodyNodeCommandResponse() {
	delete this->body;
}

int DownloadOmittedBlockBodyNodeCommandResponse::binarySize() const {
	BinaryUtils::checkNotNull(this->body);

	int total = sizeof(this->type);

	total += this->body->binarySize();

	return total;
}

void DownloadOmittedBlockBodyNodeCommandResponse::toBinary(	ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->body);

	buff->putInt(this->type);

	this->body->toBinary(buff);
}

void DownloadOmittedBlockBodyNodeCommandResponse::fromBinary(ByteBuffer *buff) {
	this->body = OmittedBlockBody::createFromBinary(buff);
	BinaryUtils::checkNotNull(this->body);
}

UnicodeString* DownloadOmittedBlockBodyNodeCommandResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"Omitted Block"); __STP(str);

	return __STP_MV(str);
}

void DownloadOmittedBlockBodyNodeCommandResponse::setOmittedBlockBody(const OmittedBlockBody *body) {
	delete this->body;
	this->body = dynamic_cast<OmittedBlockBody*>(body->copyData());
}

} /* namespace codablecash */
