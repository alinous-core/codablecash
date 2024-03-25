/*
 * CodablecashConfig.h
 *
 *  Created on: 2023/03/30
 *      Author: iizuka
 */

#ifndef BC_CODABLECASHCONFIG_H_
#define BC_CODABLECASHCONFIG_H_

#include <cstdint>

namespace codablecash {

class CodablecashConfig {
public:
	CodablecashConfig(const CodablecashConfig& inst);
	CodablecashConfig();
	virtual ~CodablecashConfig();

	virtual uint16_t getPowHashrateBlocks(uint64_t height) const noexcept;
	void setPowHashrateBlocks(uint16_t powHashrateBlocks) noexcept {
		this->powHashrateBlocks = powHashrateBlocks;
	}

	virtual uint64_t getPowBlockTimeMills(uint64_t height) const noexcept;
	void setPowBlockTimeMills(uint64_t powBlockTimeMills) noexcept {
		this->powBlockTimeMills = powBlockTimeMills;
	}

	uint16_t getPowAllowedSceduledDiffMevScore() const noexcept;
	void setPowAllowedSceduledDiffMevScore(uint16_t powAllowedSceduledDiffMevScore) noexcept;


	uint64_t getPowVoteWaitingIntervalMills(uint64_t height) const noexcept;
	void setPowVoteWaitingIntervalMills(uint64_t powVoteWaitingInterval) noexcept;

	uint16_t getVoteDefaultCapacity(uint64_t height) const noexcept;

	uint16_t getVoteMissingLimit(uint64_t height) const noexcept;
	void setVoteMissingLimit(uint16_t missingLimit) noexcept;

	uint16_t getVoteExtendCapacityCount(uint64_t height) const noexcept;
	void setVoteExtendCapacityCount(uint16_t extendCapacityCount) noexcept;

	uint16_t getTicketMatureIntervalHeight(uint64_t height) const noexcept;
	void setTicketMatureIntervalHeight(uint16_t tiketMatureIntervalHeight) noexcept;

	uint64_t getTicketExpireHeight(uint64_t height) const noexcept;
	void setTicketExpireHeight(uint64_t ticketExpireHeight) noexcept;


	uint64_t getTicketPriceDefault(uint64_t height) const noexcept;
	void setTicketPriceDefault(uint64_t ticketPriceDefault) noexcept;

	uint64_t getTicketPriceWindow(uint64_t height) const noexcept;
	void setTicketPriceWindow(uint64_t ticketPriceWindow) noexcept;

	uint64_t getTicketIdealNumber(uint64_t height) const noexcept;
	void setTicketIdealNumber(uint64_t ticketIdealNumber) noexcept;

	uint64_t getTicketVoterFeeBasisPoint(uint64_t height) const noexcept;
	void setTicketVoterFeeBasisPoint(uint64_t ticketVoterFeeBasisPoint) noexcept;

	uint16_t getVoteBeforeNBlocks(uint64_t height) const noexcept;
	void setVoteBeforeNBlocks(uint16_t voteBeforeNBlocks) noexcept;

	uint16_t getVoteBlockIncludeAfterNBlocks(uint64_t height) const noexcept;
	void setVoteBlockIncludeAfterNBlocks(uint16_t voteBlockIncludeAfterNBlocks) noexcept;

	uint8_t getVotePerBlock() const noexcept {
		return this->votePerBlock;
	}

	void setThreadsNodeTransferProcessor(uint8_t threadsNodeTransferProcessor) noexcept;
	uint8_t getThreadsNodeTransferProcessor() const noexcept {
		return this->threadsNodeTransferProcessor;
	}

	void setThreadsNetworkInfoTransferProcessor(uint8_t threadsNetworkInfoTransferProcessor) noexcept;
	uint8_t getThreadsNetworkInfoTransferProcessor() const noexcept {
		return this->threadsNetworkInfoTransferProcessor;
	}

	void setNumConnectionOwnShard(uint16_t numConnectionOwnShard) noexcept;
	uint16_t getNumConnectionOwnShard() const noexcept {
		return this->numConnectionOwnShard;
	}

	void setNumConnectionExtShard(uint16_t numConnectionExtShard) noexcept;
	uint16_t getNumConnectionExtShard() const noexcept {
		return this->numConnectionExtShard;
	}

	void setNumInitialSyncNodes(uint8_t numInitialSyncNodes) noexcept;
	uint8_t getNumInitialSyncNodes() const noexcept {
		return this->numInitialSyncNodes;
	}

private:
	// pow
	uint16_t powHashrateBlocks;
	uint64_t powBlockTimeMills;

	uint16_t powAllowedSceduledDiffMevScore;

	// voting
	uint16_t voteDefaultCapacity;
	uint16_t voteMissingLimit;
	uint64_t voteExtendCapacityCount;

	uint16_t voteBeforeNBlocks;
	uint16_t voteBlockIncludeAfterNBlocks;

	uint8_t votePerBlock;

	uint16_t ticketMatureIntervalHeight;
	uint64_t ticketExpireHeight;

	uint64_t ticketPriceDefault;
	uint64_t ticketPriceWindow;
	uint64_t ticketIdealNumber;

	uint64_t ticketVoterFeeBasisPoint;

	// network
	uint8_t threadsNodeTransferProcessor;
	uint8_t threadsNetworkInfoTransferProcessor;

	uint16_t numConnectionOwnShard;
	uint16_t numConnectionExtShard;

	// sync
	uint8_t numInitialSyncNodes;
};

} /* namespace codablecash */

#endif /* BC_CODABLECASHCONFIG_H_ */
