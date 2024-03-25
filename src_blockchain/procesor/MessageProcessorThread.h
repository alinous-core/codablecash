/*
 * MessageProcessorThread.h
 *
 *  Created on: 2022/03/07
 *      Author: iizuka
 */

#ifndef PROCESOR_MESSAGEPROCESSORTHREAD_H_
#define PROCESOR_MESSAGEPROCESSORTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

using alinous::AbstractThreadRunner;
using alinous::UnicodeString;

namespace codablecash {

class MessageProcessor;
class ICommandMessage;
class ISystemLogger;

class MessageProcessorThread : public AbstractThreadRunner {
public:
	static const constexpr int STATUS_STOPPED{0};
	static const constexpr int STATUS_WAITING{1};
	static const constexpr int STATUS_RUNNING{2};

	explicit MessageProcessorThread(MessageProcessor* processor, ISystemLogger* logger);
	MessageProcessorThread(MessageProcessor* processor, const UnicodeString* name, ISystemLogger* logger);
	virtual ~MessageProcessorThread();

	void setRunning(bool bl) noexcept;
	bool isRunning() const noexcept;

	int getStatus() const noexcept;

protected:
	virtual void process() noexcept;

private:
	void processLoop() noexcept;
	void processCommand(ICommandMessage* cmd);
	bool checkListIsEmpty();

private:
	MessageProcessor* processor;
	ISystemLogger* logger;

	volatile int status;
	volatile bool running;

};

} /* namespace codablecash */

#endif /* PROCESOR_MESSAGEPROCESSORTHREAD_H_ */
