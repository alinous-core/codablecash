/*
 * AbstractSQLStatement.cpp
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */

#include "lang_sql/sql/AbstractSQLStatement.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

namespace alinous {

AbstractSQLStatement::AbstractSQLStatement(int kind) : AbstractStatement(kind) {
}

AbstractSQLStatement::~AbstractSQLStatement() {
}

void AbstractSQLStatement::init(VirtualMachine* vm) {
}

bool AbstractSQLStatement::hasCtrlStatement() const noexcept {
	return true;
}

AbstractStatement* AbstractSQLStatement::generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) const {
	int cap = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true);
	toBinary(buff);

	buff->position(0);

	CodeElement* element = CodeElement::createFromBinary(buff); __STP(element);

	return dynamic_cast<AbstractStatement*>(__STP_MV(element));
}

} /* namespace alinous */
