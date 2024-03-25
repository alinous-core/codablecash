/*
 * MessageProcessor.h
 *
 *  Created on: 2022/03/07
 *      Author: iizuka
 */

#ifndef PROCESOR_MESSAGEPROCESSOR_H_
#define PROCESOR_MESSAGEPROCESSOR_H_

#include "base/ArrayList.h"

#include "procesor/ICommandMessage.h"

namespace alinous {
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class MessageProcessorThread;
class ICommandParameter;
class ISystemLogger;

class MessageProcessor {
public:
	explicit MessageProcessor(ISystemLogger* logger);
	MessageProcessor(const UnicodeString* name, ISystemLogger* logger);
	virtual ~MessageProcessor();

	void start();
	void shutdown() noexcept;

	SynchronizedLock* getLock() const noexcept {
		return lock;
	}

	void addCommandMessage(ICommandMessage* cmd) noexcept;
	bool __isEmpty() const noexcept;
	ICommandMessage* __fetch() noexcept;

	void setParam(ICommandParameter* param) noexcept;
	ICommandParameter* getParam() const noexcept {
		return this->param;
	}

private:
	SynchronizedLock* lock;

	ArrayList<ICommandMessage> list;

	MessageProcessorThread* thread;

	ICommandParameter* param;

	volatile bool acceptCommands;
};

} /* namespace codablecash */

#endif /* PROCESOR_MESSAGEPROCESSOR_H_ */
