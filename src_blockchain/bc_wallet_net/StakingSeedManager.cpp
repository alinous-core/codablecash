/*
 * StakingSeedManager.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/StakingSeedManager.h"

#include "base_io/File.h"


namespace codablecash {

StakingSeedManager::StakingSeedManager(const File* baseDir) {
	this->storeDir = baseDir->get(StakingSeedManager::STAKING_SEGMENT);


	// TODO Auto-generated constructor stub
}

StakingSeedManager::~StakingSeedManager() {
	delete this->storeDir;
}

} /* namespace codablecash */
