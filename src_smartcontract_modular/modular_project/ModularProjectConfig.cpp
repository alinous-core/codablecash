/*
 * ModularProjectConfig.cpp
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */

#include "modular_project/ModularProjectConfig.h"
#include "modular_project/ModularConfigException.h"

#include "json/JsonHandler.h"

#include "json_object/AbstractJsonObject.h"
#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/JsonStringValue.h"
#include "json_object/JsonArrayObject.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

ModularProjectConfig::ModularProjectConfig() {
	this->projectName = nullptr;
	this->executable = nullptr;

	this->libralies = new ArrayList<UnicodeString>();

	this->author = nullptr;
	this->license = nullptr;
}

ModularProjectConfig::~ModularProjectConfig() {
	delete this->projectName;
	delete this->executable;

	this->libralies->deleteElements();
	delete this->libralies;

	delete this->author;
	delete this->license;
}

void ModularProjectConfig::load(const File *file) {
	JsonHandler handler;
	handler.loadFile(file, 256);
	AbstractJsonObject* object = handler.parse(); __STP(object);

	const JsonObject* root = dynamic_cast<JsonObject*>(object);
	ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(root == nullptr, L"Config file must be json format.", __FILE__, __LINE__);

	// project
	{
		const JsonValuePair* pair = root->get(PROJECT);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The project is essential.", __FILE__, __LINE__);

		AbstractJsonObject* obj = pair->getValue();
		JsonStringValue* stringValue = dynamic_cast<JsonStringValue*>(obj);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(stringValue == nullptr, L"The project must be string value.", __FILE__, __LINE__);

		setProjectName(stringValue->getValue());
	}

	// executable
	{
		const JsonValuePair* pair = root->get(EXECUTABLE);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The executable is essential.", __FILE__, __LINE__);

		AbstractJsonObject* obj = pair->getValue();
		JsonStringValue* stringValue = dynamic_cast<JsonStringValue*>(obj);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(stringValue == nullptr, L"The executable must be string value.", __FILE__, __LINE__);

		setExecutable(stringValue->getValue());
	}

	// author
	{
		const JsonValuePair* pair = root->get(AUTHOR);
		if(pair != nullptr){
			AbstractJsonObject* obj = pair->getValue();
			JsonStringValue* stringValue = dynamic_cast<JsonStringValue*>(obj);
			ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(stringValue == nullptr, L"The author must be string value.", __FILE__, __LINE__);

			setAuthor(stringValue->getValue());
		}
	}

	// libraries
	{
		const JsonValuePair* pair = root->get(LIBRALIES);
		if(pair != nullptr){
			AbstractJsonObject* obj = pair->getValue();
			JsonArrayObject* arrayObj = dynamic_cast<JsonArrayObject*>(obj);
			ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(arrayObj == nullptr, L"The libraries must be array value.", __FILE__, __LINE__);

			int maxLoop = arrayObj->size();
			for(int i = 0; i != maxLoop; ++i){
				AbstractJsonObject* arobj = arrayObj->get(i);
				JsonStringValue* p = dynamic_cast<JsonStringValue*>(arobj);
				ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(p == nullptr, L"The json file's format is broken.", __FILE__, __LINE__);

				const UnicodeString* libname = p->getValue();
				addLibrary(libname);
			}
		}
	}
}

void ModularProjectConfig::setProjectName(const UnicodeString *name) noexcept {
	delete this->projectName;
	this->projectName = new UnicodeString(name);
}

void ModularProjectConfig::setExecutable(const UnicodeString *exec) noexcept {
	delete this->executable;
	this->executable = new UnicodeString(exec);
}

void ModularProjectConfig::setAuthor(const UnicodeString *author) noexcept {
	delete this->author;
	this->author = new UnicodeString(author);
}

void ModularProjectConfig::setLicense(const UnicodeString *license) noexcept {
	delete this->license;
	this->license = new UnicodeString(license);
}

void ModularProjectConfig::addLibrary(const UnicodeString *libname) noexcept {
	UnicodeString* str = new UnicodeString(libname);
	this->libralies->addElement(str);
}

} /* namespace codablecash */
