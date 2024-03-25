/*
 * CommandSubscriberThread.h
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#ifndef PUBSUB_COMMANDSUBSCRIBERTHREAD_H_
#define PUBSUB_COMMANDSUBSCRIBERTHREAD_H_

#include "base_thread/AbstractThreadRunner.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class CommandSubscriber;
class IClientSocket;

class CommandSubscriberThread : public alinous::AbstractThreadRunner {
public:
	static const constexpr int __READ_POLL_MILLS{200};
	static const constexpr int MAX_COMMAND_SIZE{20 * 1000 * 1000}; // 20MB

	explicit CommandSubscriberThread(CommandSubscriber* subscriber);
	virtual ~CommandSubscriberThread();

	static void checkSocketConnected(IClientSocket* socket);

protected:
	virtual void process() noexcept;

private:
	void handleCommand(IClientSocket* socket);

private:
	CommandSubscriber* subscriber;

#ifdef __DEBUG__
	bool thread_ends;
#endif // __DEBUG__
};

} /* namespace codablecash */

#endif /* PUBSUB_COMMANDSUBSCRIBERTHREAD_H_ */
