/*
 * DependencyConfig.h
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#ifndef MODULAR_PROJECT_DEPENDENCYCONFIG_H_
#define MODULAR_PROJECT_DEPENDENCYCONFIG_H_

#include "base/ArrayList.h"

using namespace alinous;


namespace codablecash {

class AbstractDependencyConfig;
class JsonArrayObject;

class DependencyConfig {
public:
	static constexpr const wchar_t* NAME = L"name";
	static constexpr const wchar_t* VERSION = L"version";

	DependencyConfig(const DependencyConfig& inst);
	DependencyConfig();
	virtual ~DependencyConfig();

	void load(const JsonArrayObject* dependencies);

private:
	ArrayList<AbstractDependencyConfig>* list;

};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_DEPENDENCYCONFIG_H_ */
