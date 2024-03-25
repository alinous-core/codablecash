/*
 * AbstractJoinExecutor.h
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#ifndef TRANSACTION_SCANNER_JOIN_ABSTRACTJOINEXECUTOR_H_
#define TRANSACTION_SCANNER_JOIN_ABSTRACTJOINEXECUTOR_H_

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class IJoinLeftSource;
class IJoinRightSource;
class ScanResultMetadata;
class ScanJoinContext;
class AbstractScanCondition;
class LocalOidFactory;
class CdbRecord;

class AbstractJoinExecutor : public IJoinLeftSource {
public:
	AbstractJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
			, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory);
	virtual ~AbstractJoinExecutor();

	virtual const ScanResultMetadata* getMetadata() const noexcept {
		return this->metadata;
	}

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual void shutdown();

	bool checkByFilterCondition(VirtualMachine* vm, const CdbRecord* record);


protected:
	void setLocalOid(CdbRecord* record);

protected:
	IJoinLeftSource* left;
	IJoinRightSource* right;

	ScanResultMetadata* metadata;
	ScanJoinContext* context;
	AbstractScanCondition* filterCondition;

	LocalOidFactory* localOidFactory;
};

} /* namespace codablecash */

#endif /* TRANSACTION_SCANNER_JOIN_ABSTRACTJOINEXECUTOR_H_ */
