/*
 * ArrayNetworkSheeder.h
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_ARRAYNETWORKSHEEDER_H_
#define BC_NETWORK_INSTANCE_ARRAYNETWORKSHEEDER_H_

#include "bc_network_instance/INetworkSeeder.h"

namespace codablecash {

class ArrayNetworkSheeder : public INetworkSeeder {
public:
	ArrayNetworkSheeder();
	virtual ~ArrayNetworkSheeder();

	virtual ArrayList<P2pNodeRecord>* getSeedNodes() const noexcept;

	void addRecord(const P2pNodeRecord* record) noexcept;

	virtual int getNumZones() const noexcept;

private:
	ArrayList<P2pNodeRecord>* list;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_ARRAYNETWORKSHEEDER_H_ */
