/*
 * UpdateColumnPlanner.h
 *
 *  Created on: 2022/01/08
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPLANNER_H_
#define SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPLANNER_H_

#include "base/ArrayList.h"

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class UpdateColumnPolicy;
class CdbRecord;
class ScanResultMetadata;

class UpdateColumnPlanner {
public:
	UpdateColumnPlanner();
	virtual ~UpdateColumnPlanner();

	void addPolicy(UpdateColumnPolicy* p) noexcept;

	CdbRecord* createUpdatedRecord(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

private:
	ArrayList<UpdateColumnPolicy> list;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPLANNER_H_ */
