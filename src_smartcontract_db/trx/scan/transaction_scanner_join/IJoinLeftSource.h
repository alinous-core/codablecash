/*
 * IJoinLeftSource.h
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#ifndef TRANSACTION_SCANNER_JOIN_IJOINLEFTSOURCE_H_
#define TRANSACTION_SCANNER_JOIN_IJOINLEFTSOURCE_H_

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class CdbRecord;
class ScanResultMetadata;

class IJoinLeftSource {
public:
	IJoinLeftSource();
	virtual ~IJoinLeftSource();

	virtual void start(VirtualMachine* vm) = 0;
	virtual void restartScan(VirtualMachine* vm) = 0;
	virtual bool hasNext(VirtualMachine* vm) = 0;
	virtual const CdbRecord* next(VirtualMachine* vm) = 0;

	virtual void shutdown() = 0;

	virtual const ScanResultMetadata* getMetadata() const noexcept = 0;
};

} /* namespace codablecash */

#endif /* TRANSACTION_SCANNER_JOIN_IJOINLEFTSOURCE_H_ */
