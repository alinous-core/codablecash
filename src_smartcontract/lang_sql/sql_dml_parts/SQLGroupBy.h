/*
 * SQLGroupBy.h
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */

#ifndef SQL_DML_PARTS_SQLGROUPBY_H_
#define SQL_DML_PARTS_SQLGROUPBY_H_

#include "lang_sql/sql/AbstractSQLPart.h"

namespace alinous {
class SQLColumnsList;
class SQLHaving;
class AnalyzeContext;
class AbstractVmInstance;
class VirtualMachine;

class SQLGroupBy : public AbstractSQLPart {
public:
	SQLGroupBy();
	virtual ~SQLGroupBy();

	void setList(SQLColumnsList* list) noexcept;
	void setHaving(SQLHaving* having) noexcept;

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

	void init(VirtualMachine* vm);
	AbstractVmInstance* interpret(VirtualMachine* vm);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

private:
	SQLColumnsList* list;
	SQLHaving* having;
};

} /* namespace alinous */

#endif /* SQL_DML_PARTS_SQLGROUPBY_H_ */
