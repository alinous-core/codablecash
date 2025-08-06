/*
 * AbstractPubSubCommand.cpp
 *
 *  Created on: 2023/01/07
 *      Author: iizuka
 */

#include "pubsub_cmd/AbstractPubSubCommand.h"
#include "pubsub_cmd/EchoPubSubCommand.h"
#include "pubsub_cmd/EndHandshakeCommand.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/LoginClientCommand.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"
#include "bc_p2p_cmd_client/ClientListStakingNodeIdsCommand.h"

#include "ipconnect/IClientSocket.h"

#include "pubsub/PubsubCommandException.h"
#include "pubsub/PubsubUtils.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"
#include "bc_p2p_cmd_client/PingNodeCommand.h"
#include "bc_p2p_cmd_client/SendTransactionClientCommand.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderCommand.h"
#include "bc_p2p_cmd_client_notify/ClientNotifyNewTransactionCommand.h"

#include "bc_p2p_cmd_node/SyncMempoolNodeCommand.h"
#include "bc_p2p_cmd_node/SyncHeaderNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadBlockNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadTransactionsNodeCommand.h"
#include "bc_p2p_cmd_node/NopNodeCommand.h"
#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommand.h"

#include "bc_p2p_cmd_node_consensus/ReportMinedBlockNodeCommand.h"
#include "bc_p2p_cmd_node_consensus/ReportNonceCalculatedNodeCommand.h"
#include "bc_p2p_cmd_node_consensus/SendTransactionNodeCommand.h"
#include "bc_p2p_cmd_node_consensus/SendVoteTransactionNodeCommand.h"

#include "bc_p2p_cmd_network/NodeShutdownCommand.h"


namespace codablecash {

AbstractPubSubCommand::AbstractPubSubCommand(const AbstractPubSubCommand &inst) {
	this->type = inst.type;
}

AbstractPubSubCommand::AbstractPubSubCommand(uint32_t type) {
	this->type = type;
}

AbstractPubSubCommand::~AbstractPubSubCommand() {

}

AbstractPubSubCommand* AbstractPubSubCommand::createFromBinary(ByteBuffer *buff) {
	AbstractPubSubCommand* ret = nullptr;

	uint32_t type = buff->getInt();
	switch(type){
	case TYPE_ECHO:
		ret = new EchoPubSubCommand();
		break;
	case TYPE_END_HANDSHAKE:
		ret = new EndHandshakeCommand();
		break;
	case TYPE_LOGIN:
		ret = new LoginPubSubCommand(0, nullptr);
		break;
	case TYPE_CLIENT_LOGIN:
		ret = new LoginClientCommand(0);
		break;
	case TYPE_CLIENT_DOWNLOAD_DNS:
		ret = new DownloadDnsInfoCommand(0);
		break;
	case TYPE_CLIENT_PING:
		ret = new PingNodeCommand();
		break;
	case TYPE_CLIENT_TRANSACTION_TRANSFER:
		ret = new SendTransactionClientCommand();
		break;
	case TYPE_CLIENT_SYNC_HEADER:
		ret = new ClientSyncHeaderCommand();
		break;
	case TYPE_CLIENT_LIST_STAKING_IDS:
		ret = new ClientListStakingNodeIdsCommand();
		break;
	case AbstractPubSubCommand::TYPE_NETWORK_NODE_SHUTDOWN:
		ret = new NodeShutdownCommand();
		break;
	case TYPE_NODE_NOP:
		ret = new NopNodeCommand();
		break;
	case TYPE_NODE_SEND_TRANSACTION:
		ret = new SendTransactionNodeCommand();
		break;
	case TYPE_CLIENT_NOTIFY_NEW_TRANSACTION:
		ret = new ClientNotifyNewTransactionCommand();
		break;
	case TYPE_NODE_SYNC_MEMPOOL:
		ret = new SyncMempoolNodeCommand();
		break;
	case TYPE_NODE_SYNC_HEADER:
		ret = new SyncHeaderNodeCommand();
		break;
	case TYPE_NODE_DOWNLOAD_BLOCK:
		ret = new DownloadBlockNodeCommand();
		break;
	case TYPE_NODE_REPORT_MINED_BLOCK:
		ret = new ReportMinedBlockNodeCommand();
		break;
	case TYPE_NODE_DOWNLOAD_OMITTED_BLOCK_BODY:
		ret = new DownloadOmittedBlockBodyNodeCommand();
		break;
	case TYPE_NODE_DOWNLOAD_TRANSACTIONS:
		ret = new DownloadTransactionsNodeCommand();
		break;

	case TYPE_CONSENSUS_REPORT_NONCE_CALCULATED:
		ret = new ReportNonceCalculatedNodeCommand();
		break;
	case TYPE_CONSENSUS_SEND_VOTE_TRANSACTION:
		ret = new SendVoteTransactionNodeCommand();
		break;

	default:
		throw new PubsubCommandException(L"Wrong command type.", __FILE__, __LINE__);
	}

	ret->fromBinary(buff);

	return ret;
}

void AbstractPubSubCommand::putString(ByteBuffer* out, const UnicodeString* str) const noexcept {
	PubsubUtils::putString(out, str);
}

UnicodeString* AbstractPubSubCommand::getString(ByteBuffer* in) const noexcept {
	return PubsubUtils::getString(in);
}

int AbstractPubSubCommand::stringSize(const UnicodeString* str) const noexcept {
	return PubsubUtils::stringSize(str);
}

} /* namespace codablecash */
