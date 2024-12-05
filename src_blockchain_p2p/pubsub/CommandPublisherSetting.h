/**
 * @file	CommandPublisherSetting.h
 * @date	2024/04/19
 * @author	iizuka 
 * @version 0.0.1
 */
#ifndef PUBSUB_COMMANDPUBLISHERSETTING_H_
#define PUBSUB_COMMANDPUBLISHERSETTING_H_

class CommandPublisherSetting {
public:
	CommandPublisherSetting();
	virtual ~CommandPublisherSetting();

	static CommandPublisherSetting* getInstance() noexcept;

	int getPoolRetry() const {
		return poolRetry;
	}

	void setPoolRetry(int poolRetry) {
		this->poolRetry = poolRetry;
	}

	int getWaitSec() const {
		return waitSec;
	}

	void setWaitSec(int waitSec) {
		this->waitSec = waitSec;
	}

private:
	int poolRetry;
	int waitSec;
};

class CommandPublisherSettingStack {
public:
	CommandPublisherSettingStack(int poolRetry, int waitSec);
	virtual ~CommandPublisherSettingStack();

private:
	int lastPoolRetry;
	int lastWaitSec;
};

#endif /* PUBSUB_COMMANDPUBLISHERSETTING_H_ */
