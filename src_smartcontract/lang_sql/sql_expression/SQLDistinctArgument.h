/*
 * SQLDistinctArgument.h
 *
 *  Created on: 2021/11/02
 *      Author: iizuka
 */

#ifndef LANG_SQL_SQL_EXPRESSION_SQLDISTINCTARGUMENT_H_
#define LANG_SQL_SQL_EXPRESSION_SQLDISTINCTARGUMENT_H_

#include "lang_sql/sql/AbstractSQLExpression.h"

namespace alinous {

class SQLDistinctArgument : public AbstractSQLExpression {
public:
	SQLDistinctArgument();
	virtual ~SQLDistinctArgument();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);
	virtual AnalyzedType getType(AnalyzeContext* actx);

	virtual void init(VirtualMachine* vm);
	virtual AbstractVmInstance* interpret(VirtualMachine* vm);
	virtual void onSelectTarget(VirtualMachine* vm);

	void setExpression(AbstractSQLExpression* exp) noexcept {
		this->exp = exp;
	}
private:
	AbstractSQLExpression* exp;
};

} /* namespace alinous */

#endif /* LANG_SQL_SQL_EXPRESSION_SQLDISTINCTARGUMENT_H_ */
