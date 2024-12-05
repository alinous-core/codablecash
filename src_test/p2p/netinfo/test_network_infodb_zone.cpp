/*
 * test_network_infodb_zone.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_p2p_info/P2pNodeDatabase.h"
#include "bc_p2p_info/P2pNodeRecord.h"
#include "bc_p2p_info/ZoneNodeKey.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "btreekey/NullKey.h"

using namespace codablecash;


TEST_GROUP(TestNetworkInfodbZoneGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


static void createZone(uint16_t zone, int maxLoop, P2pNodeDatabase& info){
	for(int i = 0; i != maxLoop; ++i){
		NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
		NodeIdentifier nodeId = source->toNodeIdentifier();

		{
			P2pNodeRecord* rec = info.findRecord(&nodeId); __STP(rec);
			assert(rec == nullptr);
		}


		P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(zone, &nodeId, nullptr, L"testhost", 10000); __STP(rec);
		info.addRecord(rec);
	}
}

TEST(TestNetworkInfodbZoneGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	P2pNodeDatabase info(baseDir);
	info.createBlankDatabase();

	info.open();

	createZone(0, 3, info);
	createZone(1, 4, info);
	createZone(2, 5, info);

	{
		ArrayList<NodeIdentifier>* list = info.listNodesInZone(0); __STP(list);
		list->deleteElements();
		CHECK(list->size() == 3);
	}

	{
		ArrayList<NodeIdentifier>* list = info.listNodesInZone(1); __STP(list);
		list->deleteElements();
		CHECK(list->size() == 4);
	}

	{
		ArrayList<NodeIdentifier>* list = info.listNodesInZone(2); __STP(list);
		list->deleteElements();
		CHECK(list->size() == 5);
	}


	info.close();
}

TEST(TestNetworkInfodbZoneGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	P2pNodeDatabase info(baseDir);
	info.createBlankDatabase();

	info.open();
	{
		NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
		NodeIdentifier nodeId = source->toNodeIdentifier();

		P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(0, &nodeId, nullptr, L"testhost", 10000); __STP(rec);
		info.addRecord(rec);

		rec->setZone(1);
		info.addRecord(rec);

		{
			ArrayList<NodeIdentifier>* list = info.listNodesInZone(0); __STP(list);
			list->deleteElements();
			CHECK(list->size() == 0);
		}

		{
			ArrayList<NodeIdentifier>* list = info.listNodesInZone(1); __STP(list);
			list->deleteElements();
			CHECK(list->size() == 1);
		}
	}
	info.close();
}

TEST(TestNetworkInfodbZoneGroup, misc01){
	ZoneNodeKey key;
	NullKey nkey;

	int res = key.compareTo(&nkey);
	CHECK(res > 0);
}
