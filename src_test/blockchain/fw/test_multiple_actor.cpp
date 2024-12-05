/*
 * test_multiple_actor.cpp
 *
 *  Created on: 2023/08/13
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "procesor_multi/MultipleCommandProcessor.h"

#include "procesor/BlankCommandParameter.h"
#include "procesor/NopCommandMessage.h"

#include "bc/DebugDefaultLogger.h"

#include "DummyErrorNopCommand.h"

using namespace codablecash;

TEST_GROUP(TestMultiActorFrameworkGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};


TEST(TestMultiActorFrameworkGroup, case01){
	DebugDefaultLogger logger;
	BlankCommandParameter param;
	UnicodeString name(L"th");
	MultipleCommandProcessor procesor(&param, 3, &logger, &name);
}

TEST(TestMultiActorFrameworkGroup, case02){
	DebugDefaultLogger logger;
	BlankCommandParameter param;
	UnicodeString name(L"th");
	MultipleCommandProcessor procesor(&param, 3, &logger, &name);

	procesor.start();

	procesor.shutdown();
}

TEST(TestMultiActorFrameworkGroup, case03){
	DebugDefaultLogger logger;
	BlankCommandParameter param;
	UnicodeString name(L"th");
	MultipleCommandProcessor procesor(&param, 3, &logger, &name);

	procesor.start();

	int maxLoop = 10;
	for(int i = 0; i != maxLoop; ++i){
		NopCommandMessage* cmd = new NopCommandMessage();
		procesor.addCommandMessage(cmd, i);
	}

	procesor.shutdown();

	CHECK(param.getCount() == maxLoop);
}

TEST(TestMultiActorFrameworkGroup, case04){
	DebugDefaultLogger logger;
	BlankCommandParameter param;
	UnicodeString name(L"th");
	MultipleCommandProcessor procesor(&param, 3, &logger, &name);

	procesor.start();

	int maxLoop = 10;
	for(int i = 0; i != maxLoop; ++i){
		if(i == 2){
			DummyErrorNopCommand* cmd = new DummyErrorNopCommand();
			procesor.addCommandMessage(cmd, i);
			continue;
		}

		NopCommandMessage* cmd = new NopCommandMessage();
		procesor.addCommandMessage(cmd, i);
	}

	procesor.shutdown();

	CHECK(param.getCount() == (maxLoop - 1));

}


