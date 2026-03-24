/*
 * SmartcontractCallSequence.h
 *
 *  Created on: Mar 22, 2026
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_EXECUTOR_CURSOR_SMARTCONTRACTCALLSEQUENCE_H_
#define SMARTCONTRACT_EXECUTOR_CURSOR_SMARTCONTRACTCALLSEQUENCE_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class SessionContextCursorData;

class SmartcontractCallSequence {
public:
	SmartcontractCallSequence();
	virtual ~SmartcontractCallSequence();

	void addFirst(const SessionContextCursorData* data);

private:
	ArrayList<SessionContextCursorData>* list;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_CURSOR_SMARTCONTRACTCALLSEQUENCE_H_ */
