/*
 * ModularProjectUtils.cpp
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */

#include "ModularProjectUtils.h"

#include "base_io/File.h"


namespace codablecash {

ModularProjectUtils::ModularProjectUtils(const wchar_t* seg, const File* projectFolder) {
	this->folder = projectFolder->get(seg);
}

ModularProjectUtils::~ModularProjectUtils() {
	delete this->folder;
}

} /* namespace codablecash */
