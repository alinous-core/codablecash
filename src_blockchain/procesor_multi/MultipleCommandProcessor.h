/*
 * MultipleCommandProcessor.h
 *
 *  Created on: 2023/08/13
 *      Author: iizuka
 */

#ifndef PROCESOR_MULTI_MULTIPLECOMMANDPROCESSOR_H_
#define PROCESOR_MULTI_MULTIPLECOMMANDPROCESSOR_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class CommandProcessUnit;
class ICommandParameter;
class ISystemLogger;
class ICommandMessage;

class MultipleCommandProcessor {
public:
	MultipleCommandProcessor(ICommandParameter* param, int size, ISystemLogger* logger, const wchar_t* name);
	virtual ~MultipleCommandProcessor();

	void start();
	void shutdown() noexcept;

	void addCommandMessage(ICommandMessage* cmd, int i) noexcept;

	ICommandParameter* getParam() const noexcept {
		return this->param;
	}

	int size() const noexcept {
		return list->size();
	}

private:
	ArrayList<CommandProcessUnit>* list;

	ICommandParameter* param;
	ISystemLogger* logger;
};

} /* namespace codablecash */

#endif /* PROCESOR_MULTI_MULTIPLECOMMANDPROCESSOR_H_ */
