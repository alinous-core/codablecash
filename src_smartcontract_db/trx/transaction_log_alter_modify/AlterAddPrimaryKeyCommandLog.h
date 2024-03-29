/*
 * AlterAddPrimaryKeyCommandLog.h
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ALTER_MODIFY_ALTERADDPRIMARYKEYCOMMANDLOG_H_
#define TRANSACTION_LOG_ALTER_MODIFY_ALTERADDPRIMARYKEYCOMMANDLOG_H_

#include "trx/transaction_log_alter/AbstractAlterCommandLog.h"

namespace alinous {
class AlterAddPrimaryKeyCommand;
}
using namespace alinous;

namespace codablecash {

class AlterAddPrimaryKeyCommandLog : public AbstractAlterCommandLog {
public:
	explicit AlterAddPrimaryKeyCommandLog(uint64_t oid);
	virtual ~AlterAddPrimaryKeyCommandLog();

	void setCommand(AlterAddPrimaryKeyCommand* command) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	virtual void initCommandParam(VirtualMachine* vm, TableIdentifier* tableId);

	const AlterAddPrimaryKeyCommand* getCommand() const noexcept {
		return command;
	}

private:
	AlterAddPrimaryKeyCommand* command;
};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ALTER_MODIFY_ALTERADDPRIMARYKEYCOMMANDLOG_H_ */
