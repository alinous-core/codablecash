/*
 * BooleanLiteralColumnParam.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_PARAMS_BOOLEANLITERALCOLUMNPARAM_H_
#define SCAN_COLUMNS_PARAMS_BOOLEANLITERALCOLUMNPARAM_H_

#include "scan_select/scan_columns/scan_columns_params/AbstractColumnParam.h"

namespace codablecash {

class BooleanScanParam;

class BooleanLiteralColumnParam: public AbstractColumnParam {
public:
	static const UnicodeString TRUE;
	static const UnicodeString FALSE;

	explicit BooleanLiteralColumnParam(bool value);
	virtual ~BooleanLiteralColumnParam();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);

	virtual IValueProvider* toValueProvider();

private:
	bool value;

	BooleanScanParam* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_PARAMS_BOOLEANLITERALCOLUMNPARAM_H_ */
