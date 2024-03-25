/*
 * UpdateStatement.h
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */

#ifndef SQL_DML_UPDATESTATEMENT_H_
#define SQL_DML_UPDATESTATEMENT_H_

#include "lang_sql/sql/AbstractSQLStatement.h"

namespace codablecash {
class SelectScanPlanner;
class UpdateColumnPlanner;
class CdbTransaction;
class CdbRecord;
class UpdateLog;
class CdbTableIdentifier;
class ScanResultMetadata;
}
using namespace codablecash;

namespace alinous {
class TableIdentifier;
class SQLSet;
class SQLWhere;
class VmTransactionHandler;

class UpdateStatement : public AbstractSQLStatement {
public:
	UpdateStatement();
	virtual ~UpdateStatement();

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	void setTable(TableIdentifier* tableId) noexcept;
	void setSet(SQLSet* set) noexcept;
	void setWhere(SQLWhere* where) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	virtual void init(VirtualMachine* vm);
	virtual void interpret(VirtualMachine* vm);

private:
	void buildPlanner(VirtualMachine* vm, uint64_t currentVer);
	void updateSchemaInfo(VirtualMachine* vm, VmTransactionHandler* trxHandler);

	void doUpdateScan(VirtualMachine* vm, CdbTransaction* trx);
	void doUpdate(UpdateLog* cmd, const CdbRecord* record, VirtualMachine* vm, CdbTransaction* trx, const ScanResultMetadata* metadata);

private:
	TableIdentifier* tableId;
	SQLSet* set;
	SQLWhere* where;

	uint64_t lastSchemaVersion;
	SelectScanPlanner* planner;
	UpdateColumnPlanner* updatePlanner;

	CdbTableIdentifier* tableIdentifier;
};

} /* namespace alinous */

#endif /* SQL_DML_UPDATESTATEMENT_H_ */
