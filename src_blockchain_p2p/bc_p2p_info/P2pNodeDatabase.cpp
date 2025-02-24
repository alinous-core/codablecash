/*
 * P2pNodeDatabase.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#include "bc_p2p_info/P2pNodeDatabase.h"
#include "bc_p2p_info/P2pNodeRecordDataFactory.h"
#include "bc_p2p_info/P2pNodeRecord.h"
#include "bc_p2p_info/P2pNodeRecordReference.h"
#include "bc_p2p_info/P2PZoneConnectionInfo.h"

#include "base_io/File.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc_network/NodeIdentifierKeyFactory.h"
#include "bc_network/NodeIdentifierKey.h"

#include "bc_p2p_info/NodeIdentifierDataFactory.h"

#include "bc_p2p_info/ZoneNodeKeyFactory.h"

#include "bc_p2p_info/ZoneNodeKey.h"

#include "bc_network/NodeIdentifier.h"

#include "btree/BtreeScanner.h"
namespace codablecash {

P2pNodeDatabase::P2pNodeDatabase(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->nodesStore = nullptr;
	this->zoneIndex = nullptr;
	this->cacheManager = new DiskCacheManager();
}

P2pNodeDatabase::~P2pNodeDatabase() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void P2pNodeDatabase::createBlankDatabase() {
	{
		UnicodeString fileName(MAIN_DATA);

		NodeIdentifierKeyFactory* keyFactory = new NodeIdentifierKeyFactory(); __STP(keyFactory);
		P2pNodeRecordDataFactory* dataFactory = new P2pNodeRecordDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}

	{
		UnicodeString fileName(ZONE_INDEX);

		ZoneNodeKeyFactory* keyFactory = new ZoneNodeKeyFactory(); __STP(keyFactory);
		NodeIdentifierDataFactory* dataFactory = new NodeIdentifierDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void P2pNodeDatabase::open() {
	{
		UnicodeString fileName(MAIN_DATA);

		NodeIdentifierKeyFactory* keyFactory = new NodeIdentifierKeyFactory(); __STP(keyFactory);
		P2pNodeRecordDataFactory* dataFactory = new P2pNodeRecordDataFactory(); __STP(dataFactory);

		this->nodesStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->nodesStore->open(&opconf);
	}

	{
		UnicodeString fileName(ZONE_INDEX);

		ZoneNodeKeyFactory* keyFactory = new ZoneNodeKeyFactory(); __STP(keyFactory);
		NodeIdentifierDataFactory* dataFactory = new NodeIdentifierDataFactory(); __STP(dataFactory);

		this->zoneIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->zoneIndex->open(&opconf);
	}
}

void P2pNodeDatabase::close() {
	if(this->nodesStore != nullptr){
		this->nodesStore->close();

		delete this->nodesStore;
		this->nodesStore = nullptr;
	}
	if(this->zoneIndex != nullptr){
		this->zoneIndex->close();

		delete this->zoneIndex;
		this->zoneIndex = nullptr;
	}
}

void P2pNodeDatabase::addRecord(const P2pNodeRecord *record) {
	{
		P2pNodeRecord* prev = findRecord(record->getNodeIdentifier()); __STP(prev);
		if(prev != nullptr && prev->getZone() != record->getZone()){
			removeRecord(record->getNodeIdentifier());
		}
	}
	NodeIdentifierKey key(record->getNodeIdentifier());

	this->nodesStore->putData(&key, record);

	addZoneIndex(record);
}

P2pNodeRecord* P2pNodeDatabase::findRecord(const NodeIdentifier *nodeId) {
	NodeIdentifierKey key(nodeId);

	IBlockObject* obj = this->nodesStore->findByKey(&key); __STP(obj);
	P2pNodeRecord* record = dynamic_cast<P2pNodeRecord*>(obj);

	return record != nullptr ? dynamic_cast<P2pNodeRecord*>(__STP_MV(obj)) : nullptr;
}

bool P2pNodeDatabase::removeRecord(const NodeIdentifier *nodeId) {
	{
		P2pNodeRecord* rec = findRecord(nodeId); __STP(rec);
		if(rec != nullptr){
			removeZoneIndex(rec);
		}
	}

	removeCounterPartConnections(nodeId);

	NodeIdentifierKey key(nodeId);
	return this->nodesStore->remove(&key);
}

void P2pNodeDatabase::removeCounterPartConnections(const NodeIdentifier *nodeId) {
	P2pNodeRecord* rec = findRecord(nodeId); __STP(rec);

	if(rec != nullptr){
		uint16_t zone = rec->getZone();
		ArrayList<P2PZoneConnectionInfo>* zoneList = rec->getConnectionZoneList();

		int maxLoop = zoneList->size();
		for(int i = 0; i != maxLoop; ++i){
			P2PZoneConnectionInfo* zoneInfo = zoneList->get(i);

			removeCounterPartZoneConnections(zone, nodeId, zoneInfo);
		}
	}
}

void P2pNodeDatabase::removeCounterPartZoneConnections(uint16_t zone, const NodeIdentifier *nodeId, P2PZoneConnectionInfo *zoneInfo) {
	ArrayList<P2pNodeRecordReference>* list = zoneInfo->getReferenceList();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		P2pNodeRecordReference* ref = list->get(i);

		P2pNodeRecord* record = findRecord(ref->getNodeIdentifier()); __STP(record);
		if(record != nullptr){
			record->removeConnectedNode(zone, nodeId);

			addRecord(record); // update
		}
	}
}

void P2pNodeDatabase::connect(const NodeIdentifier *nodeId, const NodeIdentifier *nodeId2) {
	P2pNodeRecord* record = findRecord(nodeId); __STP(record);
	P2pNodeRecord* record2 = findRecord(nodeId2); __STP(record2);

	if(record != nullptr && record2 != nullptr){
		record->addConnectedNode(record2->getZone(), nodeId2);
		record2->addConnectedNode(record->getZone(), nodeId);

		addRecord(record);
		addRecord(record2);
	}
}

void P2pNodeDatabase::addZoneIndex(const P2pNodeRecord *record) {
	const NodeIdentifier* nodeId = record->getNodeIdentifier();
	NodeIdentifierKey idkey(nodeId);

	ZoneNodeKey key;
	key.init(record->getZone(), &idkey);

	this->zoneIndex->putData(&key, nodeId);
}

void P2pNodeDatabase::importUnconnectedNode(const P2pNodeRecord *nodeRecord) {
	const NodeIdentifier* nodeId = nodeRecord->getNodeIdentifier();
	P2pNodeRecord* record = findRecord(nodeId); __STP(record);

	if(record == nullptr){
		P2pNodeRecord* newRecord = dynamic_cast<P2pNodeRecord*>(nodeRecord->copyData()); __STP(newRecord);
		newRecord->stripConnections();

		addRecord(newRecord);
	}
}

void P2pNodeDatabase::removeZoneIndex(const P2pNodeRecord *record) {
	const NodeIdentifier* nodeId = record->getNodeIdentifier();
	NodeIdentifierKey idkey(nodeId);

	ZoneNodeKey key;
	key.init(record->getZone(), &idkey);

	this->zoneIndex->remove(&key);
}

ArrayList<NodeIdentifier>* P2pNodeDatabase::listNodesInZone(uint16_t zone) const {
	ArrayList<NodeIdentifier>* list = new ArrayList<NodeIdentifier>(); __STP(list);

	BtreeScanner* scanner = this->zoneIndex->getScanner(); __STP(scanner);
	{
		ZoneNodeKey key;
		key.init(zone, nullptr);
		scanner->begin(&key);
	}

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const AbstractBtreeKey* keyobj = scanner->nextKey();

		const ZoneNodeKey* key = dynamic_cast<const ZoneNodeKey*>(keyobj);
		if(key->getZone() != zone){
			break;
		}

		NodeIdentifier* nodeId = dynamic_cast<NodeIdentifier*>(obj->copyData());
		list->addElement(nodeId);
	}

	return __STP_MV(list);
}

ArrayList<P2pNodeRecord>* P2pNodeDatabase::getZoneRecords(uint16_t zone,	int maxNum) {
	ArrayList<P2pNodeRecord>* list = new ArrayList<P2pNodeRecord>(); __STP(list);
	ArrayList<NodeIdentifier>* nodelist = listNodesInZone(zone); __STP(nodelist);

	list->setDeleteOnExit();
	nodelist->setDeleteOnExit();

	int maxLoop = nodelist->size();
	for(int i = 0; i != maxLoop; ++i){
		NodeIdentifier* nodeId = nodelist->get(i);

		P2pNodeRecord* record = findRecord(nodeId);
		assert(record != nullptr);

		list->addElement(record);

		if(i >= maxNum){
			break;
		}
	}

	list->setDeleteOnExit(false);
	return __STP_MV(list);
}

} /* namespace codablecash */
