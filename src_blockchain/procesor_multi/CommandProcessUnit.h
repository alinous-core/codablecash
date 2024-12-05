/*
 * CommandProcessUnit.h
 *
 *  Created on: 2023/08/13
 *      Author: iizuka
 */

#ifndef PROCESOR_MULTI_COMMANDPROCESSUNIT_H_
#define PROCESOR_MULTI_COMMANDPROCESSUNIT_H_

#include "base_thread/AbstractThreadRunner.h"

#include "base/ArrayList.h"

namespace alinous {
class SynchronizedLock;
}
using namespace alinous;

namespace codablecash {

class ISystemLogger;
class MultipleCommandProcessor;
class ICommandMessage;

class CommandProcessUnit : public AbstractThreadRunner {
public:
	static const constexpr int STATUS_STOPPED{0};
	static const constexpr int STATUS_WAITING{1};
	static const constexpr int STATUS_RUNNING{2};

	CommandProcessUnit(MultipleCommandProcessor* processor, ISystemLogger* logger, const UnicodeString* name);
	virtual ~CommandProcessUnit();

	virtual void process() noexcept;

	void setRunning(bool bl) noexcept;
	bool isRunning() const noexcept;

	int getStatus() const noexcept;

	void addCommandMessage(ICommandMessage* cmd) noexcept;
	void insertCommandMessage(ICommandMessage* cmd, int pos) noexcept;

private:
	void processCommand(ICommandMessage* cmd);
	bool checkListIsEmpty();

private:
	MultipleCommandProcessor* processor;
	ISystemLogger* logger;

	SynchronizedLock* lock;
	volatile int status;
	volatile bool running;

	ArrayList<ICommandMessage> list;
};

} /* namespace codablecash */

#endif /* PROCESOR_MULTI_COMMANDPROCESSUNIT_H_ */
