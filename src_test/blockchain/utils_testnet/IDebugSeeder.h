/*
 * IDebugSeeder.h
 *
 *  Created on: Dec 4, 2024
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_TESTNET_IDEBUGSEEDER_H_
#define BLOCKCHAIN_TESTNET_IDEBUGSEEDER_H_

#include "bc_network_instance/INetworkSeeder.h"

namespace codablecash {

class IDebugSeeder: public INetworkSeeder {
public:
	IDebugSeeder();
	virtual ~IDebugSeeder();

	virtual void addRecord(const P2pNodeRecord* record) noexcept = 0;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_TESTNET_IDEBUGSEEDER_H_ */
