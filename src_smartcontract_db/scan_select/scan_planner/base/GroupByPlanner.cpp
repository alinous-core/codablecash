/*
 * GroupByPlanner.cpp
 *
 *  Created on: 2021/05/10
 *      Author: iizuka
 */

#include "scan_select/scan_planner/base/GroupByPlanner.h"

#include "scan_select/scan_columns/ScanColumnHolder.h"

namespace codablecash {

GroupByPlanner::GroupByPlanner() {
	this->columnHolder = new ScanColumnHolder();

}

GroupByPlanner::~GroupByPlanner() {
	delete this->columnHolder;
}

void codablecash::GroupByPlanner::addColumn(AbstractScanColumnsTarget* col) {
	this->columnHolder->addColumn(col);
}


} /* namespace codablecash */
