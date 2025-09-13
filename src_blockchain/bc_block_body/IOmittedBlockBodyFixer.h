/*
 * IOmittedBlockBodyFixer.h
 *
 *  Created on: Sep 4, 2025
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_IOMITTEDBLOCKBODYFIXER_H_
#define BC_BLOCK_BODY_IOMITTEDBLOCKBODYFIXER_H_

namespace codablecash {

class MemPoolTransaction;
class DownloadTransactionEntry;
class ISystemLogger;
class AbstractBlockchainTransaction;
class TransactionId;

class IOmittedBlockBodyFixer {
public:
	IOmittedBlockBodyFixer();
	virtual ~IOmittedBlockBodyFixer();

	virtual MemPoolTransaction* begin() = 0;
	virtual void addDownloadTransactionEntry(const DownloadTransactionEntry* entry) = 0;
	virtual void downloadTransactions(ISystemLogger *logger) = 0;
	virtual const AbstractBlockchainTransaction* get(const TransactionId* trxId) const noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_IOMITTEDBLOCKBODYFIXER_H_ */
