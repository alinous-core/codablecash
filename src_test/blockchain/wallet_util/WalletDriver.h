/*
 * WalletDriver.h
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_WALLET_UTIL_WALLETDRIVER_H_
#define BLOCKCHAIN_WALLET_UTIL_WALLETDRIVER_H_

#include "base/ArrayList.h"

#include <cstdint>

#include "bc_trx_balance/BalanceTransferTransaction.h"
namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class HdWallet;
class PasswordEncoder;
class BalanceUnit;
class GenesisTransaction;
class RegisterVotePoolTransaction;
class RegisterTicketTransaction;
class NodeIdentifier;
class NodeIdentifierSource;
class WalletAccount;
class P2pClient;
class AbstractCommandResponse;
class MemoryPool;

class WalletDriver {
public:
	WalletDriver(uint16_t zone, const File* baseDir, const wchar_t* pass);
	virtual ~WalletDriver();

	void init(int numWallets);
	void initGenesis(int index, uint64_t defaultBalance);

	GenesisTransaction* createGenesisTransaction(int index, uint64_t defaultBalance);

	HdWallet* newWallet(const File* baseDir);

	bool transfer(int srcIndex, int destIndex, int destAddressIndex, const BalanceUnit amount
			, const BalanceUnit feeRate, bool feeIncluded);

	BalanceTransferTransaction* createBalanceTransferTransaction(int srcIndex, int destIndex, int destAddressIndex, const BalanceUnit amount
			, const BalanceUnit feeRate, bool feeIncluded);

	RegisterVotePoolTransaction* createRegisterVotePoolTransaction(int srcIndex, const NodeIdentifierSource *source, const BalanceUnit feeRate);
	RegisterTicketTransaction* createRegisterTicketTransaction(int srcIndex, int destIndex, int destAddressIndex, const NodeIdentifier* nodeId
			, const BalanceUnit& stakeAmount, const BalanceUnit& feeRate);

	BalanceUnit getTotalBalance(int srcIndex) const noexcept;

	void importTransaction(const AbstractBlockchainTransaction *trx);

	AddressDescriptor* getAddressDesc(int destIndex, int destAddressIndex) const;

	WalletAccount* getAccount(int walletIndex, int accountIndex);

	AbstractCommandResponse* registerVotingNode(P2pClient *client);
	AbstractCommandResponse* registerTicket(P2pClient *client, const BalanceUnit& stakeAmount);
	AbstractCommandResponse* registerTicket(P2pClient *client, const BalanceUnit& stakeAmount, MemoryPool* mempool);

	AbstractCommandResponse* transfer(int srcIndex, int destIndex, int destAddressIndex, const BalanceUnit& amount
			, const BalanceUnit& feeRate, bool feeIncluded, P2pClient* client);
	AbstractCommandResponse* transfer(int srcIndex, int destIndex, int destAddressIndex, const BalanceUnit& amount
			, const BalanceUnit& feeRate, bool feeIncluded, P2pClient* client, MemoryPool* mempool);

	AbstractCommandResponse* sendNopSmartcontractTrx(P2pClient *client);
	AbstractCommandResponse* sendNopIccTrx(P2pClient *client);

	void setStakeSource(const NodeIdentifierSource* stakeSource) noexcept;
	NodeIdentifierSource* getStakeSource() const noexcept {
		return this->stakeSource;
	}

private:
	uint16_t zone;
	File* baseDir;
	ArrayList<HdWallet> list;

	PasswordEncoder* enc;

	NodeIdentifierSource* stakeSource;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_WALLET_UTIL_WALLETDRIVER_H_ */
