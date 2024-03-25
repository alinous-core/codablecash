/*
 * UndoRegisterTransaction.h
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#ifndef TRX_SESSION_BASE_UNDOREGISTERTRANSACTION_H_
#define TRX_SESSION_BASE_UNDOREGISTERTRANSACTION_H_

namespace codablecash {

class UndoSpaceManager;

class UndoRegisterTransaction {
public:
	explicit UndoRegisterTransaction(UndoSpaceManager* undoManager);
	virtual ~UndoRegisterTransaction();

	void logAdded() noexcept;
	void commit() noexcept;

private:
	void rollback();

private:
	UndoSpaceManager* undoManager;
	bool addUndoLog;
	bool committed;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_BASE_UNDOREGISTERTRANSACTION_H_ */
