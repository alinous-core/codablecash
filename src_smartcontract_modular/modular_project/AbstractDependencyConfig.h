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
class JsonObject;

class AbstractDependencyConfig {
public:
	static constexpr const wchar_t* NAME = L"name";
	static constexpr const wchar_t* VERSION = L"version";

	AbstractDependencyConfig(const AbstractDependencyConfig& inst);
	AbstractDependencyConfig();
	virtual ~AbstractDependencyConfig();

	virtual AbstractDependencyConfig* copy() const = 0;

	static void fromJson(AbstractDependencyConfig* inst, const JsonObject* jsonObj);

	void setModuleName(const UnicodeString* name) noexcept;
	void setVersion(const ModularSmartcontractVersion* v) noexcept;

private:
	UnicodeString* moduleName;
	ModularSmartcontractVersion* version;
};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_ABSTRACTDEPENDENCYCONFIG_H_ */
