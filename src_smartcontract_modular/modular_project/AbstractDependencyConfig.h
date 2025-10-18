/*
 * AbstractDependencyConfig.h
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#ifndef MODULAR_PROJECT_ABSTRACTDEPENDENCYCONFIG_H_
#define MODULAR_PROJECT_ABSTRACTDEPENDENCYCONFIG_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class ModularSmartcontractVersion;

class AbstractDependencyConfig {
public:
	AbstractDependencyConfig();
	virtual ~AbstractDependencyConfig();


private:
	UnicodeString* moduleName;
	ModularSmartcontractVersion* version;
};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_ABSTRACTDEPENDENCYCONFIG_H_ */
