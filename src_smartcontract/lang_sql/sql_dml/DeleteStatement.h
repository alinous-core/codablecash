/*
 * DeleteStatement.h
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */

#ifndef SQL_DML_DELETESTATEMENT_H_
#define SQL_DML_DELETESTATEMENT_H_

#include "lang_sql/sql/AbstractSQLStatement.h"

namespace codablecash {
class SelectScanPlanner;
class CdbTableIdentifier;
class CdbTransaction;
}
using namespace codablecash;

namespace alinous {
class SQLFrom;
class SQLWhere;
class VmTransactionHandler;

class DeleteStatement : public AbstractSQLStatement {
public:
	DeleteStatement();
	virtual ~DeleteStatement();

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	void setFrom(SQLFrom* from) noexcept;
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

private:
	SQLFrom* from;
	SQLWhere* where;

	uint64_t lastSchemaVersion;
	SelectScanPlanner* planner;

	CdbTableIdentifier* tableIdentifier;
};

} /* namespace alinous */

#endif /* SQL_DML_DELETESTATEMENT_H_ */
