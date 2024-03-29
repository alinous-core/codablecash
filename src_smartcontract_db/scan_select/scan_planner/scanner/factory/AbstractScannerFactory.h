/*
 * AbstractScannerFactory.h
 *
 *  Created on: 2020/08/28
 *      Author: iizuka
 */

#ifndef SCAN_PLANNER_SCANNER_ABSTRACTSCANNERFACTORY_H_
#define SCAN_PLANNER_SCANNER_ABSTRACTSCANNERFACTORY_H_

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {


class IJoinLeftSource;
class IJoinRightSource;
class SelectScanPlanner;
class ScanResultMetadata;
class AbstractScanCondition;
class ScanJoinContext;

class AbstractScannerFactory {
public:
	explicit AbstractScannerFactory(const ScanResultMetadata* metadata);
	virtual ~AbstractScannerFactory();

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner){};

	virtual IJoinLeftSource* createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner) = 0;
	virtual IJoinRightSource* createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse) = 0;

	void setFilterCondition(const AbstractScanCondition* filterCondition) noexcept;

	const ScanResultMetadata* getMetadata() const {
		return metadata;
	}

protected:
	ScanResultMetadata* metadata;
	AbstractScanCondition* filterCondition;
};

} /* namespace codablecash */

#endif /* SCAN_PLANNER_SCANNER_ABSTRACTSCANNERFACTORY_H_ */
