/*
 * UpdateLog.h
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_LOG_UPDATELOG_H_
#define TRX_TRANSACTION_LOG_UPDATELOG_H_

#include "trx/transaction_log/AbstractTransactionLog.h"

#include "base/ArrayList.h"

namespace codablecash {

class CdbRecord;
class CdbTableIdentifier;

class UpdateLog: public AbstractTransactionLog {
public:
	explicit UpdateLog(uint64_t oid);
	virtual ~UpdateLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	void setTable(CdbTableIdentifier* table) noexcept;
	CdbTableIdentifier* getCdbTableIdentifier() const noexcept {
		return this->table;
	}

	void addRecord(CdbRecord* record) noexcept;
	const ArrayList<CdbRecord>* getRecords() const noexcept {
		return &records;
	}

private:
	CdbTableIdentifier* table;
	ArrayList<CdbRecord> records;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_LOG_UPDATELOG_H_ */
