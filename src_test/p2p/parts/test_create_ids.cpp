/*
 * test_create_ids.cpp
 *
 *  Created on: Sep 9, 2024
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "base/ArrayList.h"

#include "pubsub/PubSubId.h"

using namespace codablecash;

TEST_GROUP(TestCreateIdseGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static bool checkSamePubsubId(const PubSubId* psId, ArrayList<PubSubId>* list){
	bool result = false;

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		PubSubId* id = list->get(i);

		if(id == psId){
			continue;
		}

		if(psId->equals(id)){
			result = true;
			break;
		}
	}

	return result;
}

TEST(TestCreateIdseGroup, pubsubId01){
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);
}

TEST(TestCreateIdseGroup, pubsubId02){
	ArrayList<PubSubId> list;

	int maxLoop = 100;
	for(int i = 0; i != maxLoop; ++i){
		PubSubId* psId = PubSubId::createNewId();
		list.addElement(psId);
	}

	int errorcount = 0;
	for(int i = 0; i != maxLoop; ++i){
		PubSubId* psId = list.get(i);

		bool res = checkSamePubsubId(psId, &list);
		CHECK(!res);
		if(res){
			errorcount++;
		}
	}

	CHECK(errorcount == 0);

	list.deleteElements();
}

TEST(TestCreateIdseGroup, nodeid01){
	NodeIdentifierSource* keys01 = NodeIdentifierSource::create(); __STP(keys01);
	NodeIdentifierSource* keys02 = NodeIdentifierSource::create(); __STP(keys02);

	NodeIdentifier nodeId01 = keys01->toNodeIdentifier();
	NodeIdentifier nodeId02 = keys02->toNodeIdentifier();

	CHECK(!nodeId01.equals(&nodeId02));
}

static bool checkSameNodeId(const NodeIdentifierSource* keys, ArrayList<NodeIdentifierSource>* list){
	bool result = false;

	const NodeIdentifier nodeId = keys->toNodeIdentifier();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifierSource* src = list->get(i);
		const NodeIdentifier id = src->toNodeIdentifier();

		if(keys == src){
			continue;
		}

		if(nodeId.equals(&id)){
			result = true;
			break;
		}
	}

	return result;
}

TEST(TestCreateIdseGroup, nodeid02){
	ArrayList<NodeIdentifierSource> list;

	int maxLoop = 100;
	for(int i = 0; i != maxLoop; ++i){
		NodeIdentifierSource* keys = NodeIdentifierSource::create();
		list.addElement(keys);
	}

	int errorcount = 0;
	for(int i = 0; i != maxLoop; ++i){
		const NodeIdentifierSource* keys = list.get(i);

		bool res = checkSameNodeId(keys, &list);
		CHECK(!res);
		if(res){
			errorcount++;
		}
	}

	CHECK(errorcount == 0);

	list.deleteElements();
}

