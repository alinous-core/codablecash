/*
 * GroupByPlanner.h
 *
 *  Created on: 2021/05/10
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_
#define SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_

namespace codablecash {

class ScanColumnHolder;
class AbstractScanColumnsTarget;

class GroupByPlanner {
public:
	GroupByPlanner();
	virtual ~GroupByPlanner();

	void addColumn(AbstractScanColumnsTarget* col);
private:
	ScanColumnHolder* columnHolder;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_BASE_GROUPBYPLANNER_H_ */
