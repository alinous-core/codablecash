/*
 * NetworkNodeSync.h
 *
 *  Created on: 2023/11/01
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_NETWORKNODESYNC_H_
#define BC_NETWORK_INSTANCE_SYNC_NETWORKNODESYNC_H_

#include "base/ArrayList.h"

#include <cstdint>

namespace alinous{
class File;
};
using namespace alinous;

namespace codablecash {

class CodablecashNetworkNode;
class AbstractZoneSync;

class NetworkNodeSync {
public:
	static const constexpr wchar_t* SYNC_DIR{L"synctmp"};

	explicit NetworkNodeSync(CodablecashNetworkNode* node);
	virtual ~NetworkNodeSync();

	void sync();

	uint16_t getZoneSelf() const noexcept {
		return this->zoneSelf;
	}
	CodablecashNetworkNode* getCodablecashNetworkNode() const noexcept {
		return this->node;
	}

	const File* getTmpSyncDir() const noexcept {
		return this->tmpSyncDir;
	}

private:
	CodablecashNetworkNode* node;

	ArrayList<AbstractZoneSync>* zones;
	uint16_t zoneSelf;

	File* tmpSyncDir;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_NETWORKNODESYNC_H_ */
