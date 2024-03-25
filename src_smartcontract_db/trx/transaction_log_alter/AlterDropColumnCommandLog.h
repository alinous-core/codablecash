/*
 * AlterDropColumnCommandLog.h
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ALTER_ALTERDROPCOLUMNCOMMANDLOG_H_
#define TRANSACTION_LOG_ALTER_ALTERDROPCOLUMNCOMMANDLOG_H_

#include "trx/transaction_log_alter/AbstractAlterCommandLog.h"

namespace alinous {
class AlterDropColumnCommand;
}
using namespace alinous;

namespace codablecash {

class AlterDropColumnCommandLog : public AbstractAlterCommandLog {
public:
	explicit AlterDropColumnCommandLog(uint64_t oid);
	virtual ~AlterDropColumnCommandLog();

	void setCommand(AlterDropColumnCommand* command) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);

	virtual void initCommandParam(VirtualMachine* vm, TableIdentifier* tableId);

	const AlterDropColumnCommand* getCommand() const noexcept {
		return command;
	}

private:
	AlterDropColumnCommand* command;
};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ALTER_ALTERDROPCOLUMNCOMMANDLOG_H_ */
