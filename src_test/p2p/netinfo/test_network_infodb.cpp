/*
 * test_network_infodb.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_p2p_info/P2pNodeDatabase.h"
#include "bc_p2p_info/P2pNodeRecord.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_network/NodeIdentifier.h"
using namespace codablecash;


TEST_GROUP(TestNetworkInfodbGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestNetworkInfodbGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	P2pNodeDatabase info(baseDir);
	info.createBlankDatabase();

	info.open();

	{
		NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
		NodeIdentifier nodeId = source->toNodeIdentifier();

		{
			P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(0, &nodeId, L"testhost", 10000); __STP(rec);
			info.addRecord(rec);
		}

		{
			P2pNodeRecord* res = info.findRecord(&nodeId); __STP(res);
			CHECK(res != nullptr);
		}

		{
			info.removeRecord(&nodeId);
			P2pNodeRecord* res = info.findRecord(&nodeId); __STP(res);
			CHECK(res == nullptr);
		}
	}

	info.close();
}

TEST(TestNetworkInfodbGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	P2pNodeDatabase info(baseDir);
	info.createBlankDatabase();
	info.open();

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();
	NodeIdentifierSource* source2 = NodeIdentifierSource::create(); __STP(source2);
	NodeIdentifier nodeId2 = source2->toNodeIdentifier();

	{
		P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(0, &nodeId, L"testhost", 10000); __STP(rec);
		info.addRecord(rec);
	}
	{
		P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(0, &nodeId2, L"myhost", 10000); __STP(rec);
		info.addRecord(rec);
	}

	{
		info.connect(&nodeId, &nodeId2);
	}
	{
		info.connect(&nodeId, &nodeId2);
	}

	{
		P2pNodeRecord* res = info.findRecord(&nodeId); __STP(res);
		CHECK(res != nullptr);

		P2pNodeRecord* res2 = new P2pNodeRecord(*res); __STP(res2);
		P2pNodeRecord* res3 = dynamic_cast<P2pNodeRecord*>(res2->copyData()); __STP(res3);
		CHECK(res3 != nullptr);
	}

	{
		info.removeRecord(&nodeId);
		P2pNodeRecord* res = info.findRecord(&nodeId); __STP(res);
		CHECK(res == nullptr);
	}

	info.close();
}
