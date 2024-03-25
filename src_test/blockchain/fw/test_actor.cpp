/*
 * test_actor.cpp
 *
 *  Created on: 2022/03/07
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "procesor/MessageProcessor.h"
#include "procesor/BlankCommandParameter.h"
#include "procesor/NopCommandMessage.h"

#include "bc/DebugDefaultLogger.h"
using namespace codablecash;

TEST_GROUP(TestActorFrameworkGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestActorFrameworkGroup, case01){
	DebugDefaultLogger logger;
	MessageProcessor processor(&logger);
	BlankCommandParameter param;

	processor.setParam(&param);

	processor.start();

	// Os::usleep(1000 * 20);

	{
		NopCommandMessage* cmd = new NopCommandMessage();
		processor.addCommandMessage(cmd);
	}

	processor.shutdown();

	CHECK(param.getCount() == 1);
}
