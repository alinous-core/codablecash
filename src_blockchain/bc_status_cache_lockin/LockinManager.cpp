/*
 * LockinManager.cpp
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/LockinManager.h"

#include "base_io/File.h"


namespace codablecash {

LockinManager::LockinManager(const File* baseDir) {
	this->baseDir = baseDir->get(NAME_LOCKIN_DATA);

}

LockinManager::~LockinManager() {
	delete this->baseDir;
}

void LockinManager::initBlank() {
}

void LockinManager::open() {
}

void LockinManager::close() noexcept {
}

} /* namespace codablecash */
