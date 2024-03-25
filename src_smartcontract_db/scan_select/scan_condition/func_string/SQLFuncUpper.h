/*
 * SQLFuncUpper.h
 *
 *  Created on: 2023/06/05
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_STRING_SQLFUNCUPPER_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_STRING_SQLFUNCUPPER_H_

#include "scan_select/scan_condition/func/AbstractSQLFunctionBody.h"

namespace codablecash {

class SQLFuncUpper : public AbstractSQLFunctionBody {
public:
	static const constexpr wchar_t* UPPER{L"upper"};

	SQLFuncUpper();
	virtual ~SQLFuncUpper();

	virtual void analyze(AnalyzeContext* actx, SQLFunctionCall* fcall);
	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ArrayList<IValueProvider>* list, const ScanResultMetadata* metadata);
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_STRING_SQLFUNCUPPER_H_ */
