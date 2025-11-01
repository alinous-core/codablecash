/*
 * SQLSetPair.h
 *
 *  Created on: 2019/02/22
 *      Author: iizuka
 */

#ifndef SQL_DML_PARTS_SQLSETPAIR_H_
#define SQL_DML_PARTS_SQLSETPAIR_H_

#include "lang_sql/sql/AbstractSQLPart.h"

namespace codablecash {
class UpdateColumnPolicy;
}
using namespace codablecash;

namespace alinous {

class SQLColumnIdentifier;
class AbstractSQLExpression;
class AnalyzeContext;
class VirtualMachine;

class SQLSetPair : public AbstractSQLPart {
public:
	SQLSetPair();
	virtual ~SQLSetPair();

	void setColumn(SQLColumnIdentifier* colId) noexcept;
	void setExpression(AbstractSQLExpression* exp) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	void init(VirtualMachine* vm);
	UpdateColumnPolicy* makeUpdateColumnPolicy(VirtualMachine* vm);

private:
	SQLColumnIdentifier* colId;
	AbstractSQLExpression* exp;
};

} /* namespace alinous */

#endif /* SQL_DML_PARTS_SQLSETPAIR_H_ */
