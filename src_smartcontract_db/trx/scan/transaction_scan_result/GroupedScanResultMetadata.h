/*
 * GroupedScanResultMetadata.h
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCAN_RESULT_GROUPEDSCANRESULTMETADATA_H_
#define TRX_SCAN_TRANSACTION_SCAN_RESULT_GROUPEDSCANRESULTMETADATA_H_

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

namespace codablecash {

class GroupedScanResultMetadata : public ScanResultMetadata {
public:
	//GroupedScanResultMetadata(const GroupedScanResultMetadata& inst);
	explicit GroupedScanResultMetadata(const ScanResultMetadata* base);
	virtual ~GroupedScanResultMetadata();

	ScanResultMetadata* getBase() const noexcept {
		return this->base;
	}

private:
	ScanResultMetadata* base;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCAN_RESULT_GROUPEDSCANRESULTMETADATA_H_ */
