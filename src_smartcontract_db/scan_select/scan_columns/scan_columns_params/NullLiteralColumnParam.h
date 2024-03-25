/*
 * NullLiteralColumnParam.h
 *
 *  Created on: 2021/12/11
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_PARAMS_NULLLITERALCOLUMNPARAM_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_PARAMS_NULLLITERALCOLUMNPARAM_H_

#include "scan_select/scan_columns/scan_columns_params/AbstractColumnParam.h"

namespace codablecash {

class NullScanParam;

class NullLiteralColumnParam : public AbstractColumnParam {
public:
	NullLiteralColumnParam();
	virtual ~NullLiteralColumnParam();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);

	virtual IValueProvider* toValueProvider();

private:
	NullScanParam* cond;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_PARAMS_NULLLITERALCOLUMNPARAM_H_ */
