/*
 * AbstractSQLFunctionBody.h
 *
 *  Created on: 2021/10/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLFUNCTIONBODY_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLFUNCTIONBODY_H_

#include "base/ArrayList.h"

namespace alinous {
class VirtualMachine;
class UnicodeString;
class AnalyzeContext;
class SQLFunctionCall;
}
using namespace alinous;


namespace codablecash {

class AbstractCdbValue;
class CdbRecord;
class ScanResultMetadata;
class IValueProvider;

class AbstractSQLFunctionBody {
public:
	explicit AbstractSQLFunctionBody(UnicodeString* name);
	virtual ~AbstractSQLFunctionBody();

	virtual bool isAggregate() const noexcept;

	virtual void analyze(AnalyzeContext* actx, SQLFunctionCall* fcall) = 0;
	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ArrayList<IValueProvider>* list, const ScanResultMetadata* metadata) = 0;

	const UnicodeString* getName() const noexcept {
		return this->name;
	}

protected:
	UnicodeString* name;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_ABSTRACTSQLFUNCTIONBODY_H_ */
