/**
 * @file	CommandPublisherSetting.cpp
 * @date	2024/04/19
 * @author	iizuka 
 * @version 0.0.1
 */

#include "pubsub/CommandPublisherSetting.h"

CommandPublisherSetting::CommandPublisherSetting() {
	this->poolRetry = 5;
	this->waitSec = 15;
}

CommandPublisherSetting::~CommandPublisherSetting() {

}

CommandPublisherSetting* CommandPublisherSetting::getInstance() noexcept {
	static CommandPublisherSetting inst;

	return &inst;
}

CommandPublisherSettingStack::CommandPublisherSettingStack(int poolRetry, int waitSec) {
	CommandPublisherSetting* setting = CommandPublisherSetting::getInstance();
	this->lastPoolRetry = setting->getPoolRetry();
	this->lastWaitSec = setting->getWaitSec();

	setting->setPoolRetry(poolRetry);
	setting->setWaitSec(waitSec);
}

CommandPublisherSettingStack::~CommandPublisherSettingStack() {
	CommandPublisherSetting* setting = CommandPublisherSetting::getInstance();

	setting->setPoolRetry(this->lastPoolRetry);
	setting->setWaitSec(this->lastWaitSec);
}
