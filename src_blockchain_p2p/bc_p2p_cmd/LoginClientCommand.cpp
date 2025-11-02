/*
 * LoginClientCommand.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p_cmd/LoginClientCommand.h"
#include "bc_p2p_cmd/SignatureErrorResponse.h"

#include "pubsub_cmd/OkPubsubResponse.h"

#include "bc_network/NodeIdentifier.h"

#include "base/Exception.h"

#include "pubsub/IPubsubCommandListner.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_p2p/BlochchainP2pManager.h"
#include "bc_p2p/StackHandshakeReleaser.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_p2p_cmd/LoginErrorResponse.h"

#include "bc_base/BinaryUtils.h"
#include "bc_wallet_filter/BloomFilter1024.h"
namespace codablecash {

LoginClientCommand::LoginClientCommand(const LoginClientCommand &inst)
		: AbstractBlockchainLoginCommand(inst) {
	this->filterList = new ArrayList<BloomFilter1024>();

	int maxLoop = inst.filterList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BloomFilter1024* f = inst.filterList->get(i);
		addBloomFilter(f);
	}
}

LoginClientCommand::LoginClientCommand(uint16_t zone)
		: AbstractBlockchainLoginCommand(AbstractPubSubCommand::TYPE_CLIENT_LOGIN, zone, nullptr){
	this->filterList = new ArrayList<BloomFilter1024>();
}

LoginClientCommand::~LoginClientCommand() {
	this->filterList->deleteElements();
	delete this->filterList;
}

int LoginClientCommand::binarySize() const {
	int total = AbstractBlockchainLoginCommand::binarySize();

	total += sizeof(uint16_t);

	int maxLoop = this->filterList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BloomFilter1024* f = this->filterList->get(i);

		total += f->binarySize();
	}

	return total;
}

void LoginClientCommand::toBinary(ByteBuffer *buff) const {
	AbstractBlockchainLoginCommand::toBinary(buff);

	int maxLoop = this->filterList->size();
	buff->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		const BloomFilter1024* f = this->filterList->get(i);
		f->toBinary(buff);
	}
}

void LoginClientCommand::fromBinary(ByteBuffer *buff) {
	AbstractBlockchainLoginCommand::fromBinary(buff);

	int maxLoop = buff->getShort();
	BinaryUtils::checkUShortRange(maxLoop, 0, 32);

	for(int i = 0; i != maxLoop; ++i){
		BloomFilter1024* f = BloomFilter1024::createFromBinary(buff);
		this->filterList->addElement(f);
	}
}


AbstractCommandResponse* LoginClientCommand::execute(const PubSubId* pubsubId, IPubsubCommandListner *listner) const {
	try{
		checkSignature();
	}
	catch(Exception* e){
		delete e;
		return new SignatureErrorResponse();
	}

	IPubsubCommandExecutor* exec = listner->getExecutor();
	CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(exec);
	BlochchainP2pManager* p2pManager = inst->getBlochchainP2pManager();

	const NodeIdentifier* nodeId = getNodeId();

	ClientNodeHandshake* cl = p2pManager->getClientHandshakeByNodeId(nodeId);
	StackHandshakeReleaser __releasercl(cl);

	BlockchainNodeHandshake* nd = p2pManager->getNodeHandshakeByNodeId(nodeId);
	StackHandshakeReleaser __releaser(nd);

	if(cl != nullptr || nd != nullptr){
		LoginErrorResponse* res = new LoginErrorResponse();
		return res;
	}

	listner->fireExecuteCommand(pubsubId, this);

	OkPubsubResponse* res = new OkPubsubResponse();
	return res;
}

IBlockObject* LoginClientCommand::copyData() const noexcept {
	return new LoginClientCommand(*this);
}

ByteBuffer* LoginClientCommand::getSignBinary() const {
	int cap = sizeof(this->zone);
	cap += this->nodeId->binarySize();

	int maxLoop = this->filterList->size();
	for(int i = 0; i != maxLoop; ++i){
		const BloomFilter1024* f = this->filterList->get(i);
		cap += f->binarySize();
	}

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);
	buff->putShort(this->zone);
	this->nodeId->toBinary(buff);


	for(int i = 0; i != maxLoop; ++i){
		const BloomFilter1024* f = this->filterList->get(i);
		f->toBinary(buff);
	}

	buff->position(0);
	return buff;
}

void LoginClientCommand::addBloomFilter(const BloomFilter1024 *f) {
	BloomFilter1024* filter = dynamic_cast<BloomFilter1024*>(f->copyData());
	this->filterList->addElement(filter);
}

} /* namespace codablecash */
