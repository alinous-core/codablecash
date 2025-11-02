/*
 * ModularInstance.h
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#ifndef MODULAR_PROJECT_MODULARINSTANCECONFIG_H_
#define MODULAR_PROJECT_MODULARINSTANCECONFIG_H_

#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractJsonValue;
class JsonObject;
class JsonArrayObject;

class ModularInstanceConfig {
public:
	static const constexpr wchar_t* MAIN_PACKAGE = L"mainPackage";
	static const constexpr wchar_t* MAIN_CLASS = L"mainClass";
	static const constexpr wchar_t* INITIALIZER = L"initializer";
	static const constexpr wchar_t* METHOD = L"method";
	static const constexpr wchar_t* ARGS = L"args";
	static const constexpr wchar_t* EXPORT_METHODS = L"exportMethods";

	ModularInstanceConfig();
	virtual ~ModularInstanceConfig();

	void load(const JsonObject* instance);

	void setMainPackage(const UnicodeString* value) noexcept;
	void setMainClass(const UnicodeString* value) noexcept;
	void setInitializerMethod(const UnicodeString* value) noexcept;

private:
	void loadMainPackageAndClass(const JsonObject* instance);
	void loadInitializer(const JsonObject* initializer);
	void loadExportMethods(const JsonArrayObject* exportMethods);

private:
	UnicodeString* mainPackage;
	UnicodeString* mainClass;
	UnicodeString* initializerMethod;

	ArrayList<AbstractJsonValue>* initializerMethodArguments;

	ArrayList<UnicodeString>* exportMethods;

};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_MODULARINSTANCECONFIG_H_ */
