/*
 * CrossJoinScanerTarget.h
 *
 *  Created on: 2020/08/29
 *      Author: iizuka
 */

#ifndef SCAN_PLANNER_SCANNER_CROSSJOINSCANERFACTORY_H_
#define SCAN_PLANNER_SCANNER_CROSSJOINSCANERFACTORY_H_

#include "scan_select/scan_planner/scanner/factory/AbstractJoinScannerFactory.h"

namespace codablecash {

class CrossJoinScanerFactory : public AbstractJoinScannerFactory {
public:
	CrossJoinScanerFactory(const ScanResultMetadata* metadata, const AbstractScanCondition* joinCondition);
	virtual ~CrossJoinScanerFactory();

	virtual IJoinLeftSource* createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual IJoinRightSource* createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse);
};

} /* namespace codablecash */

#endif /* SCAN_PLANNER_SCANNER_CROSSJOINSCANERFACTORY_H_ */
