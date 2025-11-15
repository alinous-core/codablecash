/*
 * LocalProjectModuleDependencyConfig.cpp
 *
 *  Created on: Nov 14, 2025
 *      Author: iizuka
 */

#include "modular_project/LocalProjectModuleDependencyConfig.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/JsonStringValue.h"


namespace codablecash {

LocalProjectModuleDependencyConfig::LocalProjectModuleDependencyConfig(const LocalProjectModuleDependencyConfig &inst)
		: AbstractDependencyConfig(inst) {
	this->localloc = inst.localloc != nullptr ? new UnicodeString(inst.localloc) : nullptr;
}

LocalProjectModuleDependencyConfig::LocalProjectModuleDependencyConfig() {
	this->localloc = nullptr;
}

LocalProjectModuleDependencyConfig::~LocalProjectModuleDependencyConfig() {
	delete this->localloc;
}

AbstractDependencyConfig* LocalProjectModuleDependencyConfig::copy() const {
	return new LocalProjectModuleDependencyConfig(*this);
}

LocalProjectModuleDependencyConfig* LocalProjectModuleDependencyConfig::fromJsonObject(const JsonObject *jsonObj) {
	LocalProjectModuleDependencyConfig* inst = new LocalProjectModuleDependencyConfig(); __STP(inst);

	fromJson(inst, jsonObj);

	{
		const JsonValuePair* p = jsonObj->get(LOCALLOC); // existence is proved
		AbstractJsonObject* jvalue = p->getValue();
		JsonStringValue* jstr = dynamic_cast<JsonStringValue*>(jvalue);

		const UnicodeString* lloc = jstr->getValue();
		inst->setLocalLoc(lloc);
	}

	return __STP_MV(inst);
}

void LocalProjectModuleDependencyConfig::setLocalLoc(const UnicodeString *lloc) noexcept {
	delete this->localloc;
	this->localloc = new UnicodeString(lloc);
}

} /* namespace codablecash */
