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
class DomArrayVariable;
}  // namespace alinous

using namespace alinous;

namespace codablecash {

class IJoinLeftSource;
class CodableDatabase;
class OidKeyRecordCache;
class GroupCache;
class CdbRecord;
class ScanResultMetadata;
class RootScanCondition;
class SelectScanPlanner;
class ScanColumnHolder;
class GroupedScanResultMetadata;

class ScanResultExecutor {
public:
	explicit ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db);
	virtual ~ScanResultExecutor();

	void init(VirtualMachine* vm);
	void execScan(VirtualMachine* vm);
	void putResult(VirtualMachine* vm, SelectScanPlanner* planner);

	OidKeyRecordCache* getOidKeyRecordCache() const noexcept {
		return this->cache;
	}

private:
	void doExecScan(VirtualMachine* vm);
	bool checkRecord(VirtualMachine* vm, RootScanCondition* root, const CdbRecord* record, const ScanResultMetadata* metadata);
	DomArrayVariable* getRecordsVariable(VirtualMachine* vm, SelectScanPlanner* planner);

	void doGroupBy(VirtualMachine* vm, SelectScanPlanner* planner);
	void putResultGroupBy(VirtualMachine* vm, SelectScanPlanner* planner);
	void handleHaving(VirtualMachine* vm, SelectScanPlanner* planner, GroupedScanResultMetadata* metadata);
	DomArrayVariable* getRecordsVariableGroupBy(VirtualMachine* vm, SelectScanPlanner* planner, GroupedScanResultMetadata* metadata);

	CdbRecord* scanResultColumns(VirtualMachine* vm, ScanColumnHolder* scanColumns, const CdbRecord* record, const ScanResultMetadata* metadata);

private:
	IJoinLeftSource* source;
	CodableDatabase* db;
	OidKeyRecordCache* cache;
	GroupCache* groupKeyCache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCAN_RESULT_SCANRESULTEXECUTOR_H_ */
