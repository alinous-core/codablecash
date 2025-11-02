/*
 * SQLHaving.h
 *
 *  Created on: 2019/02/20
 *      Author: iizuka
 */

#ifndef SQL_DML_PARTS_SQLHAVING_H_
#define SQL_DML_PARTS_SQLHAVING_H_

#include "lang_sql/sql/AbstractSQLPart.h"

namespace alinous {
class AbstractSQLExpression;
class AnalyzeContext;

class VirtualMachine;
class AbstractVmInstance;

class SQLHaving : public AbstractSQLPart {
public:
	SQLHaving();
	virtual ~SQLHaving();

	void setExpression(AbstractSQLExpression* exp) noexcept;

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

	void init(VirtualMachine* vm);
	AbstractVmInstance* interpret(VirtualMachine* vm);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);
private:
	AbstractSQLExpression* exp;
};

} /* namespace alinous */

#endif /* SQL_DML_PARTS_SQLHAVING_H_ */
