/*
 * DeleteLog.h
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_LOG_DELETELOG_H_
#define TRX_TRANSACTION_LOG_DELETELOG_H_

#include "trx/transaction_log/AbstractTransactionLog.h"

#include "base/ArrayList.h"

namespace codablecash {

class CdbTableIdentifier;
class CdbOid;

class DeleteLog: public AbstractTransactionLog {
public:
	explicit DeleteLog(uint64_t oid);
	virtual ~DeleteLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	CdbTableIdentifier* getCdbTableIdentifier() const noexcept {
		return this->table;
	}
	void setTable(CdbTableIdentifier* table) noexcept {
		this->table = table;
	}
	const ArrayList<CdbOid>* getOidsList() const noexcept {
		return &this->oids;
	}

	void addOid(CdbOid* oid) noexcept;

private:
	CdbTableIdentifier* table;
	ArrayList<CdbOid> oids;
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_LOG_DELETELOG_H_ */
