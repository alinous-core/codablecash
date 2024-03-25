/*
 * test_node_command_queue.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "command_queue/CommandQueue.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "command_queue/CommandQueueData.h"
#include "command_queue_cmd/NopNodeQueueCommand.h"

#include "pubsub/PubSubId.h"


using namespace codablecash;

TEST_GROUP(TestNodeCommandQueueGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static void addQueue(CommandQueue& queue, NodeIdentifierSource* source, int num){
	NodeIdentifier nodeId = source->toNodeIdentifier();

	NopNodeQueueCommand cmd;
	cmd.setNum(num);

	CommandQueueData* data = new CommandQueueData(); __STP(data);
	PubSubId* pubsubId = PubSubId::createNewId(); __STP(pubsubId);
	data->setPublishId(pubsubId);

	data->setCommand(&cmd);

	queue.addCommand(data);
}


TEST(TestNodeCommandQueueGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CommandQueue queue(baseDir);

	queue.createBlankDatabase();

	queue.open();
	{
		NopNodeQueueCommand cmd;

		CommandQueueData* data = new CommandQueueData(); __STP(data);
		PubSubId* pubsubId = PubSubId::createNewId(); __STP(pubsubId);
		data->setPublishId(pubsubId);

		data->setCommand(&cmd);

		queue.addCommand(data);
	}
	queue.close();
}

TEST(TestNodeCommandQueueGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CommandQueue queue(baseDir);

	queue.createBlankDatabase();

	queue.open();
	{
		addQueue(queue, source, 1);
		addQueue(queue, source, 2);
		addQueue(queue, source, 3);

		CommandQueueData* first = dynamic_cast<CommandQueueData*>(queue.getFirst()); __STP(first);
		const NopNodeQueueCommand* cmd = dynamic_cast<const NopNodeQueueCommand*>(first->getCommand());
		CHECK(cmd->getNum() == 1);

		CommandQueueData* last = dynamic_cast<CommandQueueData*>(queue.getLast()); __STP(last);
		cmd = dynamic_cast<const NopNodeQueueCommand*>(last->getCommand());
		CHECK(cmd->getNum() == 3);
	}
	queue.close();
}

TEST(TestNodeCommandQueueGroup, case03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CommandQueue queue(baseDir);

	queue.createBlankDatabase();

	queue.open();
	{
		bool bl = queue.isEmpty();
		CHECK(bl == true);

		addQueue(queue, source, 1);
		addQueue(queue, source, 2);
		addQueue(queue, source, 3);

		bl = queue.isEmpty();
		CHECK(bl == false);

		while(!queue.isEmpty()){
			CommandQueueData* cmd = dynamic_cast<CommandQueueData*>(queue.fetchFirst()); __STP(cmd);
		}
	}
	queue.close();
}
