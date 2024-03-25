/*
 * VoteTransactionIdCertificate.h
 *
 *  Created on: 2023/11/06
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_VOTETRANSACTIONIDCERTIFICATE_H_
#define DATA_HISTORY_DATA_VOTETRANSACTIONIDCERTIFICATE_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class VoteBlockTransaction;
class MerkleCertificate;

class VoteTransactionIdCertificate : public IBlockObject {
public:
	VoteTransactionIdCertificate(const VoteTransactionIdCertificate& inst);
	VoteTransactionIdCertificate();
	virtual ~VoteTransactionIdCertificate();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static VoteTransactionIdCertificate* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setVoteBlockTransaction(const VoteBlockTransaction* trx);
	void setMerkleCertificate(const MerkleCertificate* cert);

	const VoteBlockTransaction* getVoteBlockTransaction() const noexcept {
		return this->voteTrx;
	}
	const MerkleCertificate* getMerkleCertificate() const noexcept {
		return this->cert;
	}

private:
	VoteBlockTransaction* voteTrx;
	MerkleCertificate* cert;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_VOTETRANSACTIONIDCERTIFICATE_H_ */
