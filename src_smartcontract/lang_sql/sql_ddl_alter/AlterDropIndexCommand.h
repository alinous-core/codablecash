/*
 * AlterDropIndexCommand.h
 *
 *  Created on: 2020/09/08
 *      Author: iizuka
 */

#ifndef SQL_DDL_ALTER_ALTERDROPINDEXCOMMAND_H_
#define SQL_DDL_ALTER_ALTERDROPINDEXCOMMAND_H_

#include "lang_sql/sql_ddl_alter/AbstractAlterDdlCommand.h"

namespace alinous {

class UnicodeString;

class AlterDropIndexCommand : public AbstractAlterDdlCommand {
public:
	AlterDropIndexCommand(const AlterDropIndexCommand& inst);
	AlterDropIndexCommand();
	virtual ~AlterDropIndexCommand();

	void setName(UnicodeString* name) noexcept;
	const UnicodeString* getName() const noexcept {
		return name;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractAlterCommandLog* getCommandLog(VirtualMachine* vm);

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);
	virtual void interpret(VirtualMachine* vm, AbstractAlterCommandLog* log, TableIdentifier* tableId);

private:
	UnicodeString* name;
};

} /* namespace alinous */

#endif /* SQL_DDL_ALTER_ALTERDROPINDEXCOMMAND_H_ */
