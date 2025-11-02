/*
 * ModularInstance.cpp
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#include "modular_project/ModularInstanceConfig.h"
#include "modular_project/ModularConfigException.h"

#include "json_object/AbstractJsonValue.h"
#include "json_object/JsonObject.h"
#include "json_object/JsonValuePair.h"
#include "json_object/JsonArrayObject.h"
#include "json_object/JsonStringValue.h"

#include "base/UnicodeString.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

ModularInstanceConfig::ModularInstanceConfig() {
	this->mainPackage = nullptr;
	this->mainClass = nullptr;
	this->initializerMethod = nullptr;

	this->initializerMethodArguments = new ArrayList<AbstractJsonValue>();
	this->exportMethods = new ArrayList<UnicodeString>();

}

ModularInstanceConfig::~ModularInstanceConfig() {
	delete this->mainPackage;
	delete this->mainClass;
	delete this->initializerMethod;

	this->initializerMethodArguments->deleteElements();
	delete this->initializerMethodArguments;

	this->exportMethods->deleteElements();
	delete this->exportMethods;
}

void ModularInstanceConfig::load(const JsonObject *instance) {
	// mainPackage mainClass
	loadMainPackageAndClass(instance);

	// initializer
	{
		const JsonValuePair* pair = instance->get(INITIALIZER);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The initializer is essential.", __FILE__, __LINE__);

		AbstractJsonObject* object = pair->getValue();
		JsonObject* initializer = dynamic_cast<JsonObject*>(object);

		if(initializer != nullptr){
			loadInitializer(initializer);
		}
	}

	// exportMethods
	{
		const JsonValuePair* pair = instance->get(EXPORT_METHODS);
		if(pair != nullptr){
			AbstractJsonObject* object = pair->getValue();
			JsonArrayObject* exportMethods = dynamic_cast<JsonArrayObject*>(object);

			if(exportMethods != nullptr){
				loadExportMethods(exportMethods);
			}
		}
	}
}

void ModularInstanceConfig::loadMainPackageAndClass(const JsonObject *instance) {
	// mainPackage
	{
		const JsonValuePair* pair = instance->get(MAIN_PACKAGE);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The mainPackage is essential.", __FILE__, __LINE__);

		AbstractJsonObject* object = pair->getValue();
		JsonStringValue* jvalue = dynamic_cast<JsonStringValue*>(object);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(jvalue == nullptr, L"The mainPackage must be string value.", __FILE__, __LINE__);

		const UnicodeString* value = jvalue->getValue();
		setMainPackage(value);
	}

	// mainClass
	{
		const JsonValuePair* pair = instance->get(MAIN_CLASS);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The mainClass is essential.", __FILE__, __LINE__);

		AbstractJsonObject* object = pair->getValue();
		JsonStringValue* jvalue = dynamic_cast<JsonStringValue*>(object);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(jvalue == nullptr, L"The mainPackage must be string value.", __FILE__, __LINE__);

		const UnicodeString* value = jvalue->getValue();
		setMainClass(value);
	}
}

void ModularInstanceConfig::loadInitializer(const JsonObject *initializer) {
	// method
	{
		const JsonValuePair* pair = initializer->get(METHOD);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The method is essential.", __FILE__, __LINE__);

		AbstractJsonObject* object = pair->getValue();
		JsonStringValue* jvalue = dynamic_cast<JsonStringValue*>(object);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(jvalue == nullptr, L"The method must be string value.", __FILE__, __LINE__);

		const UnicodeString* value = jvalue->getValue();
		setInitializerMethod(value);
	}

	// args
	{
		const JsonValuePair* pair = initializer->get(ARGS);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(pair == nullptr, L"The args is essential.", __FILE__, __LINE__);

		AbstractJsonObject* object = pair->getValue();
		JsonArrayObject* args = dynamic_cast<JsonArrayObject*>(object);
		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(args == nullptr, L"The args must be array value.", __FILE__, __LINE__);

		int maxLoop = args->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractJsonObject* object = args->get(i);
			AbstractJsonValue* value = dynamic_cast<AbstractJsonValue*>(object);

			ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(value == nullptr, L"The member of args must be value.", __FILE__, __LINE__);

			this->initializerMethodArguments->addElement(dynamic_cast<AbstractJsonValue*>(value->copy()));
		}
	}
}

void ModularInstanceConfig::loadExportMethods(const JsonArrayObject *exportMethods) {
	int maxLoop = exportMethods->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractJsonObject* jobj = exportMethods->get(i);
		JsonStringValue* stringValue = dynamic_cast<JsonStringValue*>(jobj);

		ExceptionThrower<ModularConfigException>::throwExceptionIfCondition(stringValue == nullptr, L"The exportMethods must be string value.", __FILE__, __LINE__);

		const UnicodeString* method = stringValue->getValue();
		this->exportMethods->addElement(new UnicodeString(method));
	}
}

void ModularInstanceConfig::setMainPackage(const UnicodeString *value) noexcept {
	delete this->mainPackage;
	this->mainPackage = new UnicodeString(value);
}

void ModularInstanceConfig::setMainClass(const UnicodeString *value) noexcept {
	delete this->mainClass;
	this->mainClass = new UnicodeString(value);
}

void ModularInstanceConfig::setInitializerMethod(const UnicodeString *value) noexcept {
	delete this->initializerMethod;
	this->initializerMethod = new UnicodeString(value);
}

} /* namespace codablecash */
