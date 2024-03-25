/*
 * RightOuterJoinScanTarget.h
 *
 *  Created on: 2020/08/09
 *      Author: iizuka
 */

#ifndef SCAN_TABLE_RIGHTOUTERJOINSCANTARGET_H_
#define SCAN_TABLE_RIGHTOUTERJOINSCANTARGET_H_

#include "scan_select/scan_table/AbstractJoinScanTarget.h"

namespace codablecash {

class RightOuterJoinScanTarget : public AbstractJoinScanTarget {
public:
	RightOuterJoinScanTarget();
	virtual ~RightOuterJoinScanTarget();

	virtual const UnicodeString* toString() noexcept;

	virtual AbstractScannerFactory* getScanFactory(VirtualMachine* vm, SelectScanPlanner* planner);
};

} /* namespace codablecash */

#endif /* SCAN_TABLE_RIGHTOUTERJOINSCANTARGET_H_ */
