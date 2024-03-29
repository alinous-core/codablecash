/*
 * AlterRenameColumnCommandLog.h
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMECOLUMNCOMMANDLOG_H_
#define TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMECOLUMNCOMMANDLOG_H_

#include "trx/transaction_log_alter/AbstractAlterCommandLog.h"

namespace alinous {
class AlterRenameColumnCommand;
}
using namespace alinous;

namespace codablecash {

class AlterRenameColumnCommandLog : public AbstractAlterCommandLog {
public:
	explicit AlterRenameColumnCommandLog(uint64_t oid);
	virtual ~AlterRenameColumnCommandLog();

	void setCommand(AlterRenameColumnCommand* command) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	virtual void initCommandParam(VirtualMachine* vm, TableIdentifier* tableId);

	const AlterRenameColumnCommand* getCommand() const {
		return command;
	}

private:
	AlterRenameColumnCommand* command;
};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ALTER_MODIFY_ALTERRENAMECOLUMNCOMMANDLOG_H_ */
