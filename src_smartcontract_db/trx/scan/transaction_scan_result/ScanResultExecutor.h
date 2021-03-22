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

class ScanResultExecutor {
public:
	explicit ScanResultExecutor(IJoinLeftSource* source, CodableDatabase* db);
	virtual ~ScanResultExecutor();

	void init(VirtualMachine* vm);
	void execScan(VirtualMachine* vm);

private:
	void doExecScan(VirtualMachine* vm);
private:
	IJoinLeftSource* source;
	CodableDatabase* db;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCAN_RESULT_SCANRESULTEXECUTOR_H_ */
