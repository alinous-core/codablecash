/*
 * SQLColumnsList.h
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */

#ifndef SQL_DML_PARTS_SQLCOLUMNSLIST_H_
#define SQL_DML_PARTS_SQLCOLUMNSLIST_H_

#include "lang_sql/sql/AbstractSQLPart.h"
#include "base/ArrayList.h"

namespace alinous {
class SQLColumnIdentifier;
class AnalyzeContext;
class VirtualMachine;

class SQLColumnsList : public AbstractSQLPart {
public:
	SQLColumnsList();
	virtual ~SQLColumnsList();

	void addColumn(SQLColumnIdentifier* colId) noexcept;
	int numColumns() const noexcept;

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

	void init(VirtualMachine* vm);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	const ArrayList<SQLColumnIdentifier>* getList() const {
		return &this->list;
	}

private:
	ArrayList<SQLColumnIdentifier> list;
};

} /* namespace alinous */

#endif /* SQL_DML_PARTS_SQLCOLUMNSLIST_H_ */
