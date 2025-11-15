/*
 * AbstractDependencyConfig.cpp
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#include "modular_project/AbstractDependencyConfig.h"
#include "modular_project/ModularSmartcontractVersion.h"
#include "modular_project/ModularConfigException.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/JsonStringValue.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {


AbstractDependencyConfig::AbstractDependencyConfig(const AbstractDependencyConfig &inst) {
	this->moduleName = new UnicodeString(inst.moduleName);
	this->version = new ModularSmartcontractVersion(*inst.version);
}

AbstractDependencyConfig::AbstractDependencyConfig() {
	this->moduleName = nullptr;
	this->version = nullptr;
}

AbstractDependencyConfig::~AbstractDependencyConfig() {
	delete this->moduleName;
	delete this->version;
}

void AbstractDependencyConfig::fromJson(AbstractDependencyConfig *inst, const JsonObject *jsonObj) {
	{
		const JsonValuePair* pair = jsonObj->get(NAME);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The name is essential for dependency config.", __FILE__, __LINE__);

		AbstractJsonObject* obj = pair->getValue();
		JsonStringValue* str = dynamic_cast<JsonStringValue*>(obj);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The name of dependency config must be string value.", __FILE__, __LINE__);

		const UnicodeString* strcValue = str->getValue();
		inst->setModuleName(strcValue);
	}

	{
		const JsonValuePair* pair = jsonObj->get(VERSION);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The version is essential for dependency config.", __FILE__, __LINE__);

		AbstractJsonObject* obj = pair->getValue();
		JsonStringValue* str = dynamic_cast<JsonStringValue*>(obj);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The name of dependency config must be string value.", __FILE__, __LINE__);

		const UnicodeString* strValue = str->getValue();
		ModularSmartcontractVersion* ver = ModularSmartcontractVersion::parseString(strValue); __STP(ver);
		inst->setVersion(ver);
	}
}

void AbstractDependencyConfig::setModuleName(const UnicodeString *name) noexcept {
	delete this->moduleName;
	this->moduleName = new UnicodeString(name);
}

void AbstractDependencyConfig::setVersion(const ModularSmartcontractVersion *v) noexcept {
	delete this->version;
	this->version = new ModularSmartcontractVersion(*v);
}

} /* namespace codablecash */
