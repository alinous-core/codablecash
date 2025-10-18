/*
 * ModularProjectUtils.h
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_MODULAR_UTILS_MODULARPROJECTUTILS_H_
#define SMARTCONTRACT_MODULAR_UTILS_MODULARPROJECTUTILS_H_

namespace alinous {
class File;
}
using namespace alinous;


namespace codablecash {

class ModularProjectUtils {
public:
	ModularProjectUtils(const wchar_t* seg, const File* projectFolder);
	virtual ~ModularProjectUtils();

	const File* getFolder() const noexcept {
		return this->folder;
	}

private:
	File* folder;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_MODULAR_UTILS_MODULARPROJECTUTILS_H_ */
