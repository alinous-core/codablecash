/*
 * DebugCodablecashConfigSetup.cpp
 *
 *  Created on: 2023/05/04
 *      Author: iizuka
 */

#include "DebugCodablecashConfigSetup.h"

#include "bc/CodablecashConfig.h"

namespace codablecash {

void DebugCodablecashConfigSetup::setupConfig01(CodablecashConfig &config) {
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(250);
	config.setPowAllowedSceduledDiffMevScore(1);

	config.setVoteMissingLimit(1);
	config.setTicketMatureIntervalHeight(2);
	config.setTicketExpireHeight(4);
	config.setVoteExtendCapacityCount(4);

	config.setTicketPriceDefault(2);
	config.setTicketPriceWindow(3);
	config.setTicketIdealNumber(100);

	config.setTicketVoterFeeBasisPoint(50);

	config.setVoteBeforeNBlocks(1);
	config.setVoteBlockIncludeAfterNBlocks(2);

	config.setThreadsNodeTransferProcessor(2);
	config.setThreadsNetworkInfoTransferProcessor(2);
}

void DebugCodablecashConfigSetup::setupConfig02(CodablecashConfig &config) {
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(50);

	config.setVoteMissingLimit(1);
	config.setTicketMatureIntervalHeight(2);
	config.setTicketExpireHeight(4);
	config.setVoteExtendCapacityCount(4);

	config.setTicketPriceDefault(2);
	config.setTicketPriceWindow(3);
	config.setTicketIdealNumber(100);

	config.setTicketVoterFeeBasisPoint(50);

	config.setVoteBeforeNBlocks(1);
	config.setVoteBlockIncludeAfterNBlocks(2);

	config.setThreadsNodeTransferProcessor(2);
	config.setThreadsNetworkInfoTransferProcessor(2);

	config.setNumConnectionOwnShard(2);
	config.setNumConnectionExtShard(2);

	config.setNumInitialSyncNodes(3);
}


} /* namespace codablecash */
