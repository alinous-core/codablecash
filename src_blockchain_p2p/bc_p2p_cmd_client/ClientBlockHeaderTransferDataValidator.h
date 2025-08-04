/*
 * ClientBlockHeaderTransferDataValidator.h
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATAVALIDATOR_H_
#define BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATAVALIDATOR_H_

#include "bc_block/BlockHeader.h"

namespace codablecash {

class ClientBlockHeaderTransferData;
class BlockHeaderId;
class TransactionId;

class ClientBlockHeaderTransferDataValidator : public IVoteTransactionIdCertificatevisitor {
public:
	ClientBlockHeaderTransferDataValidator(const ClientBlockHeaderTransferData* data);
	virtual ~ClientBlockHeaderTransferDataValidator();

	virtual void visit(const BlockHeaderId* votedHeaderId, const TransactionId* voteTrxId);

private:
	const ClientBlockHeaderTransferData* data;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATAVALIDATOR_H_ */
