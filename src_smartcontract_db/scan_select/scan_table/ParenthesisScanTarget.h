/*
 * ParenthesisScanTarget.h
 *
 *  Created on: 2020/08/09
 *      Author: iizuka
 */

#ifndef SCAN_TABLE_PARENTHESISSCANTARGET_H_
#define SCAN_TABLE_PARENTHESISSCANTARGET_H_

#include "scan_select/scan_table/AbstractJoinScanTarget.h"

namespace codablecash {

class ParenthesisScanTarget : public AbstractJoinScanTarget {
public:
	ParenthesisScanTarget();
	virtual ~ParenthesisScanTarget();

	virtual const UnicodeString* toString() noexcept;

	void setInner(AbstractScanTableTarget* inner) noexcept;

	virtual void resolveTable(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual void collectScanTargets(VirtualMachine* vm, SelectScanPlanner* planner, ArrayList<AbstractScanTableTarget>* list);
	virtual bool hasTarget(const AbstractScanTableTarget* target) const noexcept;

	virtual ScanTableColumnParam* findTableColumns(const UnicodeString* colName) const;


	virtual AbstractScannerFactory* getScanFactory(VirtualMachine* vm, SelectScanPlanner* planner);


private:
	AbstractScanTableTarget* inner;
};

} /* namespace codablecash */

#endif /* SCAN_TABLE_PARENTHESISSCANTARGET_H_ */
