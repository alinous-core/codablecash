/*
 * LibrarySmartcontractModule.cpp
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */

#include "modular_project/LibrarySmartcontractModule.h"
#include "modular_project/ModularConfigException.h"

#include "json/JsonHandler.h"

#include "json_object/AbstractJsonObject.h"
#include "json_object/JsonObject.h"
#include "json_object/JsonStringValue.h"
#include "json_object/JsonValuePair.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

LibrarySmartcontractModule::LibrarySmartcontractModule(const UnicodeString* projectRelativePath)
		: AbstractSmartcontractModule(projectRelativePath) {
	this->libraryName = nullptr;
}

LibrarySmartcontractModule::~LibrarySmartcontractModule() {
	delete this->libraryName;
}

void LibrarySmartcontractModule::load(const File *modulePath) {
	File* configJason = modulePath->get(CONFIG_JSON); __STP(configJason);

	JsonHandler handler;
	handler.loadFile(configJason, 256);
	AbstractJsonObject* object = handler.parse(); __STP(object);

	const JsonObject* root = dynamic_cast<JsonObject*>(object);
	ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(root == nullptr, L"Config file must be json format.", __FILE__, __LINE__);

	// libraryName
	{
		const JsonValuePair* pair = root->get(LIBRARY_NAME);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The libraryName is essential.", __FILE__, __LINE__);

		JsonStringValue* strValue = dynamic_cast<JsonStringValue*>(pair->getValue());
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(strValue == nullptr, L"The libraryName must be String.", __FILE__, __LINE__);
	}

	analyzeJsonObject(root);
}

void LibrarySmartcontractModule::setLibraryName(const UnicodeString *str) noexcept {
	delete this->libraryName;
	this->libraryName = new UnicodeString(str);
}

} /* namespace codablecash */
