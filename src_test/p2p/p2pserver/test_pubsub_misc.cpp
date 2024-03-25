/*
 * test_pubsub_misc.cpp
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "pubsub/PubSubId.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "pubsub/HandshakeConnectionException.h"
#include "pubsub/PubsubCommandException.h"

#include "pubsub_cmd/AbstractCommandResponse.h"
#include "pubsub_cmd/AbstractPubSubCommand.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_network/NodeIdentifierKey.h"

#include "btreekey/NullKey.h"

#include "btreekey/InfinityKey.h"
using namespace codablecash;

TEST_GROUP(PubsubMiscTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(PubsubMiscTestGroup, case01){
	PubSubId* pid = PubSubId::createNewId(); __STP(pid);

	int size = pid->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
	pid->toBinary(buff);

	buff->position(0);
	PubSubId* pid2 = PubSubId::fromBinary(buff); __STP(pid2);

	bool bl = pid->equals(pid2);
	CHECK(bl);
}

TEST(PubsubMiscTestGroup, testex01){
	testException<HandshakeConnectionException>();
	testException<PubsubCommandException>();
}

TEST(PubsubMiscTestGroup, createbin01){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(100, true); __STP(buff);
	buff->putInt(-1);

	PubsubCommandException* ex = nullptr;
	try{
		AbstractCommandResponse* res = AbstractCommandResponse::createFromBinary(buff);
	}
	catch(PubsubCommandException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(PubsubMiscTestGroup, createbin02){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(100, true); __STP(buff);
	buff->putInt(-1);

	PubsubCommandException* ex = nullptr;
	try{
		AbstractPubSubCommand* cmd = AbstractPubSubCommand::createFromBinary(buff);
	}
	catch(PubsubCommandException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(PubsubMiscTestGroup, networkid01){
	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	NodeIdentifierKey key(&nodeId);

	NullKey nkey;
	InfinityKey infKey;

	CHECK(key.compareTo(&nkey) > 0);
	CHECK(key.compareTo(&infKey) < 0);
}

