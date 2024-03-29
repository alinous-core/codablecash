/*
 * AlterRenameTableCommandLog.h
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMETABLECOMMANDLOG_H_
#define TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMETABLECOMMANDLOG_H_

#include "trx/transaction_log_alter/AbstractAlterCommandLog.h"

namespace alinous {
class AlterRenameTableCommand;
}
using namespace alinous;

namespace codablecash {

class AlterRenameTableCommandLog : public AbstractAlterCommandLog {
public:
	explicit AlterRenameTableCommandLog(uint64_t oid);
	virtual ~AlterRenameTableCommandLog();

	void setCommand(AlterRenameTableCommand* command) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	virtual void initCommandParam(VirtualMachine* vm, TableIdentifier* tableId);

	const AlterRenameTableCommand* getCommand() const noexcept {
		return command;
	}

	void inputDefaultSchema(const UnicodeString* defaultSchema);
private:
	AlterRenameTableCommand* command;
};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMETABLECOMMANDLOG_H_ */
