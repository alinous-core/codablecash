/*
 * ScanResultExecutor.h
 *
 *  Created on: 2021/01/21
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCAN_RESULT_SCANRESULTEXECUTOR_H_
#define TRX_SCAN_TRANSACTION_SCAN_RESULT_SCANRESULTEXECUTOR_H_

namespace alinous {
class VirtualMachine;
}  // namespace alinous

using namespace alinous;

namespace codablecash {

class IJoinLeftSource;
class CodableDatabase;
class OidKeyRecordCache;
class CdbRecord;
class ScanResultMetadata;
class RootScanCondition;

class ScanResultExecutor {
public:
	explicit ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db);
	virtual ~ScanResultExecutor();

	void init(VirtualMachine* vm);
	void execScan(VirtualMachine* vm);
	void putResult(VirtualMachine* vm);

private:
	void doExecScan(VirtualMachine* vm);
	bool checkRecord(VirtualMachine* vm, RootScanCondition* root, const CdbRecord* record, const ScanResultMetadata* metadata);

private:
	IJoinLeftSource* source;
	CodableDatabase* db;
	OidKeyRecordCache* cache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCAN_RESULT_SCANRESULTEXECUTOR_H_ */
