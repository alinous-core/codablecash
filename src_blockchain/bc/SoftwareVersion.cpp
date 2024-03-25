/*
 * SoftwareVersion.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc/SoftwareVersion.h"

namespace codablecash {

SoftwareVersion::SoftwareVersion(int major, int minor, int patch) {
	this->major = major;
	this->minor = minor;
	this->patch = patch;
}

SoftwareVersion::~SoftwareVersion() {

}

uint64_t SoftwareVersion::toLongValue() const noexcept {
	uint64_t v = this->major;
	v = v << 16;
	v = v + this->minor;
	v = v << 16;
	v = v + this->patch;

	return v;
}

} /* namespace codablecash */
