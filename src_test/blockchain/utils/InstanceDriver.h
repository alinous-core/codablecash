/*
 * InstanceDriver.h
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_INSTANCEDRIVER_H_
#define BLOCKCHAIN_UTILS_INSTANCEDRIVER_H_
#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class WalletDriver;
class DebugDefaultLogger;
class CodablecashSystemParam;
class CodablecashNodeInstance;
class MiningConfig;
class NodeIdentifierSource;
class BalanceUnit;
class NodeIdentifier;
class IInstanceMemberAllocator;
class BalanceTransferTransaction;
class AbstractBalanceTransaction;
class AbstractBlockchainTransaction;
class BlockchainController;
class RegisterTicketTransaction;
class RegisterVotePoolTransaction;
class P2pClient;
class MemoryPool;

class InstanceDriver {
public:
	explicit InstanceDriver(const File* baseDir);
	virtual ~InstanceDriver();

	void initWallet(int numWallet);
	void initInstance(const CodablecashSystemParam* config);
	void initInstance(CodablecashSystemParam* config, const IInstanceMemberAllocator* alloc);
	void initGenesisBalance(int index,	uint64_t defaultBalance);

	void startV6Listner(int port);

	void sendBalance(BalanceUnit num, BalanceUnit feeRate);
	BalanceTransferTransaction* createBalanceTransferTransaction(BalanceUnit num, BalanceUnit feeRate);
	void transferAbstractBalanceTransaction(const AbstractBalanceTransaction* trx);

	bool validateTransaction(const AbstractBlockchainTransaction* trx) const;

	void sentTransaction(const AbstractBlockchainTransaction* trx);

	void startMiner(MiningConfig* mconfig);
	void suspendMiner();
	void resumeMiner();

	void startFinalize();

	void registerVotingNode(P2pClient* client);
	void registerVotingNode();

	void waitUntilVotingNodeRegistered();


	RegisterVotePoolTransaction* createRegisterVotePoolTransaction();
	void registerTicket(const BalanceUnit& stakeAmount);
	void registerTicket(P2pClient* client, const BalanceUnit stakeAmount);
	void registerTicket(P2pClient* client, const BalanceUnit stakeAmount, MemoryPool* pool);

	RegisterTicketTransaction* createRegisterTicketTransaction(const BalanceUnit stakeAmount);

	void waitUntilFinalizedHeight(uint64_t height);
	static void waitUntilFinalizedHeight(uint64_t height, CodablecashNodeInstance* inst);

	void waitUntilHeight(uint64_t height);

	DebugDefaultLogger* getLogger() {
		return this->logger;
	}

	CodablecashNodeInstance* getInstance() const noexcept {
		return this->inst;
	}

	BlockchainController* getBlockchainController() const noexcept;
	WalletDriver* getWallet() const noexcept {
		return this->walletDriver;
	}

private:
	File* walletDir;
	File* instDir;

	DebugDefaultLogger* logger;
	WalletDriver* walletDriver;

	CodablecashNodeInstance* inst;

	NodeIdentifierSource* source;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_INSTANCEDRIVER_H_ */
