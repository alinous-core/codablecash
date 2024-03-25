/*
 * CodablecashConfig.cpp
 *
 *  Created on: 2023/03/30
 *      Author: iizuka
 */

#include "bc/CodablecashConfig.h"

#include "cassert"
namespace codablecash {

CodablecashConfig::CodablecashConfig(const CodablecashConfig &inst) {
	this->powHashrateBlocks = inst.powHashrateBlocks;
	this->powBlockTimeMills = inst.powBlockTimeMills;
	this->powAllowedSceduledDiffMevScore = inst.powAllowedSceduledDiffMevScore;

	this->voteDefaultCapacity = inst.voteDefaultCapacity;
	this->voteMissingLimit = inst.voteMissingLimit;
	this->voteExtendCapacityCount = inst.voteDefaultCapacity;

	this->ticketMatureIntervalHeight = inst.ticketMatureIntervalHeight;
	this->ticketExpireHeight = inst.ticketExpireHeight;

	this->ticketPriceDefault = inst.ticketPriceDefault;
	this->ticketPriceWindow = inst.ticketPriceWindow;
	this->ticketIdealNumber = inst.ticketIdealNumber;

	this->ticketVoterFeeBasisPoint = inst.ticketVoterFeeBasisPoint;

	this->voteBeforeNBlocks = inst.voteBeforeNBlocks;
	this->voteBlockIncludeAfterNBlocks = inst.voteBlockIncludeAfterNBlocks;

	this->votePerBlock = inst.votePerBlock;

	this->threadsNodeTransferProcessor = inst.threadsNodeTransferProcessor;
	this->threadsNetworkInfoTransferProcessor = inst.threadsNetworkInfoTransferProcessor;

	this->numConnectionOwnShard = inst.numConnectionOwnShard;
	this->numConnectionExtShard = inst.numConnectionExtShard;

	this->numInitialSyncNodes = inst.numInitialSyncNodes;
}

CodablecashConfig::CodablecashConfig() {
	this->powHashrateBlocks = 256;
	this->powBlockTimeMills = 1000 * 60 * 3;
	this->powAllowedSceduledDiffMevScore = 10;

	this->voteDefaultCapacity = 10;
	this->voteMissingLimit = 2;
	this->voteExtendCapacityCount = 10;

	this->ticketMatureIntervalHeight = 256;
	this->ticketExpireHeight = 20 * 24 * 30 * 3;

	this->ticketPriceDefault = 2;
	this->ticketPriceWindow = 256;
	this->ticketIdealNumber = 40000;

	this->ticketVoterFeeBasisPoint = 50;

	this->voteBeforeNBlocks = 1;
	this->voteBlockIncludeAfterNBlocks = 2; // must be more than 2

	this->votePerBlock = 5;

	this->threadsNodeTransferProcessor = 8;
	this->threadsNetworkInfoTransferProcessor = 8;

	this->numConnectionOwnShard = 8;
	this->numConnectionExtShard = 4;

	this->numInitialSyncNodes = 3;
}

CodablecashConfig::~CodablecashConfig() {

}

uint16_t CodablecashConfig::getPowHashrateBlocks(uint64_t height) const noexcept {
	return this->powHashrateBlocks;
}

uint64_t CodablecashConfig::getPowBlockTimeMills(uint64_t height) const noexcept {
	return this->powBlockTimeMills;
}

uint16_t CodablecashConfig::getPowAllowedSceduledDiffMevScore() const noexcept {
	return this->powAllowedSceduledDiffMevScore;
}

void CodablecashConfig::setPowAllowedSceduledDiffMevScore(uint16_t powAllowedSceduledDiffMevScore) noexcept {
	this->powAllowedSceduledDiffMevScore = powAllowedSceduledDiffMevScore;
}

uint16_t CodablecashConfig::getVoteDefaultCapacity(uint64_t height) const noexcept {
	return this->voteDefaultCapacity;
}

uint16_t CodablecashConfig::getVoteMissingLimit(uint64_t height) const noexcept {
	return this->voteMissingLimit;
}

void CodablecashConfig::setVoteMissingLimit(uint16_t missingLimit) noexcept {
	this->voteMissingLimit = missingLimit;
}

uint16_t CodablecashConfig::getVoteExtendCapacityCount(uint64_t height) const noexcept {
	return this->voteExtendCapacityCount;
}

void CodablecashConfig::setVoteExtendCapacityCount(uint16_t extendCapacityCount) noexcept {
	this->voteExtendCapacityCount = extendCapacityCount;
}

uint16_t CodablecashConfig::getTicketMatureIntervalHeight(uint64_t height) const noexcept {
	return this->ticketMatureIntervalHeight;
}

void CodablecashConfig::setTicketMatureIntervalHeight(uint16_t tiketMatureIntervalHeight) noexcept {
	this->ticketMatureIntervalHeight = tiketMatureIntervalHeight;
}

uint64_t CodablecashConfig::getTicketExpireHeight(uint64_t height) const noexcept {
	return this->ticketExpireHeight;
}

void CodablecashConfig::setTicketExpireHeight(uint64_t ticketExpireHeight) noexcept {
	this->ticketExpireHeight = ticketExpireHeight;
}

uint64_t CodablecashConfig::getTicketPriceDefault(uint64_t height) const noexcept {
	return this->ticketPriceDefault;
}

void CodablecashConfig::setTicketPriceDefault(uint64_t ticketPriceDefault) noexcept {
	this->ticketPriceDefault = ticketPriceDefault;
}

uint64_t CodablecashConfig::getTicketPriceWindow(uint64_t height) const noexcept {
	return this->ticketPriceWindow;
}

void CodablecashConfig::setTicketPriceWindow(	uint64_t ticketPriceWindow) noexcept {
	this->ticketPriceWindow = ticketPriceWindow;
}

uint64_t CodablecashConfig::getTicketIdealNumber(uint64_t height) const noexcept {
	return this->ticketIdealNumber;
}

void CodablecashConfig::setTicketIdealNumber(	uint64_t ticketIdealNumber) noexcept {
	this->ticketIdealNumber = ticketIdealNumber;
}

uint16_t CodablecashConfig::getVoteBeforeNBlocks(uint64_t height) const noexcept {
	return this->voteBeforeNBlocks;
}

uint64_t CodablecashConfig::getTicketVoterFeeBasisPoint(uint64_t height) const noexcept {
	return this->ticketVoterFeeBasisPoint;
}

void CodablecashConfig::setTicketVoterFeeBasisPoint(	uint64_t ticketVoterFeeBasisPoint) noexcept {
	this->ticketVoterFeeBasisPoint = ticketVoterFeeBasisPoint;
}


void CodablecashConfig::setVoteBeforeNBlocks(uint16_t voteBeforeNBlocks) noexcept {
	this->voteBeforeNBlocks = voteBeforeNBlocks;
}

uint16_t CodablecashConfig::getVoteBlockIncludeAfterNBlocks(uint64_t height) const noexcept {
	return this->voteBlockIncludeAfterNBlocks;
}

void CodablecashConfig::setVoteBlockIncludeAfterNBlocks(uint16_t voteBlockIncludeAfterNBlocks) noexcept {
	assert(voteBlockIncludeAfterNBlocks >= 2);
	this->voteBlockIncludeAfterNBlocks = voteBlockIncludeAfterNBlocks;
}

void CodablecashConfig::setThreadsNodeTransferProcessor(uint8_t threadsNodeTransferProcessor) noexcept {
	this->threadsNodeTransferProcessor = threadsNodeTransferProcessor;
}

void CodablecashConfig::setThreadsNetworkInfoTransferProcessor(uint8_t threadsNetworkInfoTransferProcessor) noexcept {
	this->threadsNetworkInfoTransferProcessor = threadsNetworkInfoTransferProcessor;
}

void CodablecashConfig::setNumConnectionOwnShard(uint16_t numConnectionOwnShard) noexcept {
	this->numConnectionOwnShard = numConnectionOwnShard;
}

void CodablecashConfig::setNumConnectionExtShard(uint16_t numConnectionExtShard) noexcept {
	this->numConnectionExtShard = numConnectionExtShard;
}

void CodablecashConfig::setNumInitialSyncNodes(uint8_t numInitialSyncNodes) noexcept {
	this->numInitialSyncNodes = numInitialSyncNodes;
}

} /* namespace codablecash */
