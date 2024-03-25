/*
 * RightJoinScannerFactory.h
 *
 *  Created on: 2021/12/28
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_FACTORY_RIGHTJOINSCANNERFACTORY_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_FACTORY_RIGHTJOINSCANNERFACTORY_H_

#include "scan_select/scan_planner/scanner/factory/AbstractJoinScannerFactory.h"

namespace codablecash {

class RightJoinScannerFactory : public AbstractJoinScannerFactory {
public:
	RightJoinScannerFactory(const ScanResultMetadata* metadata, const AbstractScanCondition* joinCondition);
	virtual ~RightJoinScannerFactory();

	virtual IJoinLeftSource* createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual IJoinRightSource* createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse);
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_FACTORY_RIGHTJOINSCANNERFACTORY_H_ */
