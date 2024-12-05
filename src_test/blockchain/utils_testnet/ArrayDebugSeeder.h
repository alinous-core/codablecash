/*
 * ArrayDebugSeeder.h
 *
 *  Created on: Dec 4, 2024
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_TESTNET_ARRAYDEBUGSEEDER_H_
#define BLOCKCHAIN_UTILS_TESTNET_ARRAYDEBUGSEEDER_H_

#include "IDebugSeeder.h"

namespace codablecash {

class ArrayDebugSeeder: public IDebugSeeder {
public:
	ArrayDebugSeeder();
	virtual ~ArrayDebugSeeder();

	virtual ArrayList<P2pNodeRecord>* getSeedNodes() const noexcept;

	virtual void addRecord(const P2pNodeRecord* record) noexcept;

private:
	ArrayList<P2pNodeRecord>* list;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_TESTNET_ARRAYDEBUGSEEDER_H_ */
