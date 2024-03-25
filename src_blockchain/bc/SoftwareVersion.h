/*
 * SoftwareVersion.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_SOFTWAREVERSION_H_
#define BC_SOFTWAREVERSION_H_

#include <cstdint>

namespace codablecash {

class SoftwareVersion {
public:
	SoftwareVersion(int major, int minor, int patch);
	virtual ~SoftwareVersion();

	int getMajor() const noexcept {
		return major;
	}
	int getMinor() const noexcept {
		return minor;
	}
	int getPatch() const noexcept {
		return patch;
	}

	uint64_t toLongValue() const noexcept;

private:
	int major;
	int minor;
	int patch;
};

} /* namespace codablecash */

#endif /* BC_SOFTWAREVERSION_H_ */
