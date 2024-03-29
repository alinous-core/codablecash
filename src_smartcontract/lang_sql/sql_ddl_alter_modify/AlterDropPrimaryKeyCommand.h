/*
 * AlterDropPrimaryKeyCommand.h
 *
 *  Created on: 2020/09/09
 *      Author: iizuka
 */

#ifndef SQL_DDL_ALTER_MODIFY_ALTERDROPPRIMARYKEYCOMMAND_H_
#define SQL_DDL_ALTER_MODIFY_ALTERDROPPRIMARYKEYCOMMAND_H_

#include "lang_sql/sql_ddl_alter/AbstractAlterDdlCommand.h"

namespace alinous {

class AlterDropPrimaryKeyCommand : public AbstractAlterDdlCommand {
public:
	AlterDropPrimaryKeyCommand(const AlterDropPrimaryKeyCommand& inst);
	AlterDropPrimaryKeyCommand();
	virtual ~AlterDropPrimaryKeyCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractAlterCommandLog* getCommandLog(VirtualMachine* vm);

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);
	virtual void interpret(VirtualMachine* vm, AbstractAlterCommandLog* log, TableIdentifier* tableId);
};

} /* namespace alinous */

#endif /* SQL_DDL_ALTER_MODIFY_ALTERDROPPRIMARYKEYCOMMAND_H_ */
