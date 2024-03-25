/*
 * SQLFunctionRegistory.cpp
 *
 *  Created on: 2021/10/28
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func/SQLFunctionRegistory.h"

#include "scan_select/scan_condition/func/AbstractSQLFunctionBody.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/func_aggregate/CountSQLFunction.h"

#include "scan_select/scan_condition/func_string/SQLFuncUpper.h"
namespace codablecash {

SQLFunctionRegistory::SQLFunctionRegistory() {
	init();
}

SQLFunctionRegistory::~SQLFunctionRegistory() {
	Iterator<UnicodeString>* it = this->map.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const UnicodeString* key = it->next();
		AbstractSQLFunctionBody* func = this->map.get(key);

		delete func;
	}
}

void SQLFunctionRegistory::init() {
	// add functions
	addFunc(new CountSQLFunction());

	addFunc(new SQLFuncUpper());
}

AbstractSQLFunctionBody* SQLFunctionRegistory::getFunction(const UnicodeString* name) const noexcept {
	UnicodeString* lowcase = name->toLowerCase(); __STP(lowcase);
	AbstractSQLFunctionBody* f = this->map.get(lowcase);

	return f;
}

void SQLFunctionRegistory::addFunc(AbstractSQLFunctionBody* funcBody) {
	this->map.put(funcBody->getName(), funcBody);
}

} /* namespace codablecash */
