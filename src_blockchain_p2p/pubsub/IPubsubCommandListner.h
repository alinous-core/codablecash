/*
 * ICommandListner.h
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#ifndef PUBSUB_IPUBSUBCOMMANDLISTNER_H_
#define PUBSUB_IPUBSUBCOMMANDLISTNER_H_

namespace alinous {
class Exception;
}
using namespace alinous;

namespace codablecash {

class AbstractPubSubCommand;
class IPubsubCommandExecutor;
class PubSubId;

class IPubsubCommandListner {
public:
	IPubsubCommandListner();
	virtual ~IPubsubCommandListner();

	virtual void fireExecuteCommand(const PubSubId* pubsubId, const AbstractPubSubCommand* cmd) = 0;
	virtual void fireOnExeptionOnSubscriber(const PubSubId* pubsubId, const Exception* e) = 0;
	virtual void fireOnSubscribeEnds(const PubSubId* pubsubId) = 0;

	virtual IPubsubCommandExecutor* getExecutor() const noexcept = 0;
};

} /* namespace codablecash */

#endif /* PUBSUB_IPUBSUBCOMMANDLISTNER_H_ */
