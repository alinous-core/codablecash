/*
 * UndoRegisterTransaction.cpp
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#include "trx/session/base/UndoRegisterTransaction.h"
#include "trx/session/base/UndoSpaceManager.h"

namespace codablecash {

UndoRegisterTransaction::UndoRegisterTransaction(UndoSpaceManager* undoManager) {
	this->undoManager = undoManager;
	this->committed = false;
	this->addUndoLog = false;
}

UndoRegisterTransaction::~UndoRegisterTransaction() {
	if(!this->committed && this->addUndoLog){
		rollback();
	}

	this->undoManager = nullptr;
}

void UndoRegisterTransaction::commit() noexcept {
	this->committed = true;
}

void UndoRegisterTransaction::logAdded() noexcept {
	this->addUndoLog = true;
}

void UndoRegisterTransaction::rollback() {
	this->undoManager->rollbackLastCommand();
}

} /* namespace codablecash */
