/*
 * P2PZoneConnectionInfo.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p_info/P2PZoneConnectionInfo.h"
#include "bc_p2p_info/P2pNodeRecordReference.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

P2PZoneConnectionInfo::P2PZoneConnectionInfo(const P2PZoneConnectionInfo &inst) {
	this->list = new ArrayList<P2pNodeRecordReference>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = inst.list->get(i);

		this->list->addElement(new P2pNodeRecordReference(*ref));
	}
}

P2PZoneConnectionInfo::P2PZoneConnectionInfo() {
	this->list = new ArrayList<P2pNodeRecordReference>();
}

P2PZoneConnectionInfo::~P2PZoneConnectionInfo() {
	this->list->deleteElements();
	delete this->list;
}

int P2PZoneConnectionInfo::binarySize() const {
	int total = 0;

	int maxLoop = this->list->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = this->list->get(i);

		total += ref->binarySize();
	}

	return total;
}

void P2PZoneConnectionInfo::toBinary(ByteBuffer *out) const {
	int maxLoop = this->list->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = this->list->get(i);

		ref->toBinary(out);
	}
}

P2PZoneConnectionInfo* P2PZoneConnectionInfo::fromBinary(ByteBuffer *in) {
	P2PZoneConnectionInfo* inst = new P2PZoneConnectionInfo();

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = P2pNodeRecordReference::fromBinary(in); __STP(ref);
		BinaryUtils::checkNotNull(ref);

		inst->list->addElement(__STP_MV(ref));
	}

	return inst;
}

void P2PZoneConnectionInfo::addConnectedNode(const NodeIdentifier *nodeId) {
	P2pNodeRecordReference* ref = getConnectedNode(nodeId);
	if(ref != nullptr){
		ref->updateLastUpdatedTime();
	}
	else{
		ref = new P2pNodeRecordReference(nodeId);
		this->list->addElement(ref);
	}
}

P2pNodeRecordReference* P2PZoneConnectionInfo::getConnectedNode(const NodeIdentifier *nodeId) const noexcept {
	P2pNodeRecordReference* ret = nullptr;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = this->list->get(i);

		if(nodeId->compareTo(ref->getNodeIdentifier()) == 0){
			ret = ref;
			break;
		}
	}

	return ret;
}

void P2PZoneConnectionInfo::removeConnectedNode(const NodeIdentifier *nodeId) {
	int index = -1;
	bool found = false;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = this->list->get(i);

		if(nodeId->compareTo(ref->getNodeIdentifier()) == 0){
			index = i;
			found = true;
			break;
		}
	}

	if(found){
		delete this->list->remove(index);
	}
}

} /* namespace codablecash */
