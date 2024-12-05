/*
 * AbstarctVoteLockinOperation.h
 *
 *  Created on: Oct 13, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_ABSTARCTVOTELOCKINOPERATION_H_
#define BC_STATUS_CACHE_LOCKIN_ABSTARCTVOTELOCKINOPERATION_H_

#include "bc_status_cache_lockin/AbstractLockinOperation.h"

namespace codablecash {

class NodeIdentifier;
class UtxoId;

class AbstarctVoteLockinOperation : public AbstractLockinOperation {
public:
	AbstarctVoteLockinOperation(const AbstarctVoteLockinOperation& inst);
	explicit AbstarctVoteLockinOperation(uint16_t type);
	virtual ~AbstarctVoteLockinOperation();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	void setNodeId(const NodeIdentifier* nodeId);
	void setUtxoId(const UtxoId* utxoId);

protected:
	virtual bool dataEquals(const AbstractLockinOperation* op) const noexcept;

protected:
	NodeIdentifier* nodeId;
	UtxoId* utxoId;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_ABSTARCTVOTELOCKINOPERATION_H_ */
