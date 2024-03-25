/*
 * GroupedScanResultMetadata.cpp
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#include "trx/scan/transaction_scan_result/GroupedScanResultMetadata.h"

namespace codablecash {
/*
GroupedScanResultMetadata::GroupedScanResultMetadata(const GroupedScanResultMetadata& inst)
 : ScanResultMetadata(inst) {
	this->base = new ScanResultMetadata(*inst.base);
}*/

GroupedScanResultMetadata::GroupedScanResultMetadata(const ScanResultMetadata* base) : ScanResultMetadata() {
	this->base = new ScanResultMetadata(*base);
}

GroupedScanResultMetadata::~GroupedScanResultMetadata() {
	delete this->base;
}

} /* namespace codablecash */
