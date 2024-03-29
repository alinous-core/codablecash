/*
 * NumberLiteralColumnParam.h
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_PARAMS_NUMBERLITERALCOLUMNPARAM_H_
#define SCAN_COLUMNS_PARAMS_NUMBERLITERALCOLUMNPARAM_H_

#include "scan_select/scan_columns/scan_columns_params/AbstractColumnParam.h"
#include <cstdlib>

namespace codablecash {

class NumberLiteralColumnParam : public AbstractColumnParam {
public:
	explicit NumberLiteralColumnParam(int64_t longv);
	virtual ~NumberLiteralColumnParam();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);

	virtual IValueProvider* toValueProvider();

private:
	int64_t longv;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_PARAMS_NUMBERLITERALCOLUMNPARAM_H_ */
