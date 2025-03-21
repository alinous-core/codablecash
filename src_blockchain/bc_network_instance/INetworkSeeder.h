/*
 * INetworkSeeder.h
 *
 *  Created on: 2023/08/21
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_INETWORKSEEDER_H_
#define BC_NETWORK_INSTANCE_INETWORKSEEDER_H_

#include "base/ArrayList.h"
#include <cstdint>

using namespace alinous;

namespace codablecash {

class P2pNodeRecord;

class INetworkSeeder {
public:
	INetworkSeeder();
	virtual ~INetworkSeeder();

	virtual ArrayList<P2pNodeRecord>* getSeedNodes() const noexcept = 0;
	virtual void addRecord(const P2pNodeRecord* record) noexcept = 0;

	virtual int getNumZones() const noexcept = 0;

};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_INETWORKSEEDER_H_ */
