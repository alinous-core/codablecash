/*
 * SQLSet.h
 *
 *  Created on: 2019/02/20
 *      Author: iizuka
 */

#ifndef SQL_DML_PARTS_SQLSET_H_
#define SQL_DML_PARTS_SQLSET_H_

#include "lang_sql/sql/AbstractSQLPart.h"
#include "base/ArrayList.h"

namespace codablecash {
class UpdateColumnPlanner;
}  // namespace codablecash
using namespace codablecash;

namespace alinous {

class SQLSetPair;
class AnalyzeContext;
class VirtualMachine;

class SQLSet : public AbstractSQLPart {
public:
	SQLSet();
	virtual ~SQLSet();

	void addPair(SQLSetPair* pair) noexcept;

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	void init(VirtualMachine* vm);

	UpdateColumnPlanner* makeUpdateColumnPlanner(VirtualMachine* vm);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);
private:
	ArrayList<SQLSetPair> list;
};

} /* namespace alinous */

#endif /* SQL_DML_PARTS_SQLSET_H_ */
