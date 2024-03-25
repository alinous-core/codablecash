/*
 * GroupByPlanner.h
 *
 *  Created on: 2021/05/10
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_
#define SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_

namespace alinous {
class UnicodeString;
class VirtualMachine;
}

using namespace alinous;

namespace codablecash {

class ScanColumnHolder;
class AbstractScanColumnsTarget;
class SelectScanPlanner;
class CdbRecord;
class CdbGroupedRecord;
class ScanResultMetadata;
class GroupedScanResultMetadata;

class GroupByPlanner {
public:
	GroupByPlanner();
	virtual ~GroupByPlanner();

	void addColumn(AbstractScanColumnsTarget* col);

	void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	CdbRecord* groupBy(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);
	GroupedScanResultMetadata* getMetadata(const ScanResultMetadata* base);

	bool isGroupBy() const noexcept;
private:
	ScanColumnHolder* columnHolder;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_ */
