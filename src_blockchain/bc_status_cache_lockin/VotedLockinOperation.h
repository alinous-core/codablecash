/*
 * VotedLockonOperation.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_
#define BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_

#include "bc_status_cache_lockin/AbstarctVoteLockinOperation.h"


namespace codablecash {

class VotedLockinOperation : public AbstarctVoteLockinOperation {
public:
	VotedLockinOperation(const VotedLockinOperation& inst);
	VotedLockinOperation();
	virtual ~VotedLockinOperation();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void apply(const BlockHeader *header, IStatusCacheContext* context);

	virtual IBlockObject* copyData() const noexcept;

protected:
	virtual bool dataEquals(const AbstractLockinOperation* op) const noexcept;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_ */
