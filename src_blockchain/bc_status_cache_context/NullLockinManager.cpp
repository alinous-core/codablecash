/*
 * NullLockinManager.cpp
 *
 *  Created on: Oct 13, 2024
 *      Author: iizuka
 */

#include "bc_status_cache_context/NullLockinManager.h"

#include "bc_status_cache_lockin/LockinManager.h"


namespace codablecash {

NullLockinManager::NullLockinManager(LockinManager* lockinManager) {
	this->lockinManage = lockinManager;
}

NullLockinManager::~NullLockinManager() {

}

void NullLockinManager::addOperation(uint64_t height, const AbstractLockinOperation *op) {
	// do nothing
}

uint64_t NullLockinManager::getFinalizingHeight() const noexcept {
	return 0;
}

LockInOperationData* NullLockinManager::getOperantions(uint64_t height) {
	return this->lockinManage->getOperantions(height);
}

} /* namespace codablecash */
