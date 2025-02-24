/*
 * P2pNodeDatabase.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_P2PNODEDATABASE_H_
#define BC_P2P_INFO_P2PNODEDATABASE_H_

#include <cstdint>

#include "base/ArrayList.h"

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
}
using namespace alinous;

namespace codablecash {

class P2pNodeRecord;
class NodeIdentifier;
class P2PZoneConnectionInfo;

class P2pNodeDatabase {
public:
	static const constexpr wchar_t* MAIN_DATA{L"p2pnodes"};
	static const constexpr wchar_t* ZONE_INDEX{L"p2pzoneindex"};

	explicit P2pNodeDatabase(const File* baseDir);
	virtual ~P2pNodeDatabase();

	void createBlankDatabase();
	void open();
	void close();

	void addRecord(const P2pNodeRecord* record);
	P2pNodeRecord* findRecord(const NodeIdentifier* nodeId);
	bool removeRecord(const NodeIdentifier* nodeId);

	void connect(const NodeIdentifier* nodeId, const NodeIdentifier* nodeId2);

	ArrayList<NodeIdentifier>* listNodesInZone(uint16_t zone) const;

	ArrayList<P2pNodeRecord>* getZoneRecords(uint16_t zone, int maxNum);

	void importUnconnectedNode(const P2pNodeRecord *nodeRecord);

	void updateP2pRecord(const P2pNodeRecord *nodeRecord);

private:
	void removeCounterPartConnections(const NodeIdentifier* nodeId);
	void removeCounterPartZoneConnections(uint16_t zone, const NodeIdentifier* nodeId, P2PZoneConnectionInfo* zoneInfo);

	void addZoneIndex(const P2pNodeRecord *record);
	void removeZoneIndex(const P2pNodeRecord *record);

private:
	File* baseDir;

	DiskCacheManager* cacheManager;
	Btree* nodesStore;
	Btree* zoneIndex;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_P2PNODEDATABASE_H_ */
