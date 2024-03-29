/*
 * TableScannerFactory.h
 *
 *  Created on: 2020/08/29
 *      Author: iizuka
 */

#ifndef SCAN_PLANNER_SCANNER_TABLESCANNERFACTORY_H_
#define SCAN_PLANNER_SCANNER_TABLESCANNERFACTORY_H_

#include "scan_select/scan_planner/scanner/factory/AbstractScannerFactory.h"

namespace codablecash {

class CdbTable;
class TableStore;
class AbstractColumnsIndexAccess;
class CdbTransaction;
class AbstractScanTableTarget;
class AbstractJoinCandidate;

class TableScannerFactory : public AbstractScannerFactory {
public:
	explicit TableScannerFactory(AbstractScanTableTarget* target, const CdbTable* table, const ScanResultMetadata* metadata,
			AbstractColumnsIndexAccess* indexCandidate);
	virtual ~TableScannerFactory();

	virtual IJoinLeftSource* createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual IJoinRightSource* createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse);

private:
	IJoinLeftSource* createIndexScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner, TableStore* tableStore, CdbTransaction* trx,
			AbstractColumnsIndexAccess* indexCandidate);
	IJoinRightSource* createIndexScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, AbstractJoinCandidate* joinCandidate, bool leftReverse);


private:
	AbstractColumnsIndexAccess* indexCandidate;
	const CdbTable* table;
	AbstractScanTableTarget* target;
};

} /* namespace codablecash */

#endif /* SCAN_PLANNER_SCANNER_TABLESCANNERFACTORY_H_ */
