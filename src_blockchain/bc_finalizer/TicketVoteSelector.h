/*
 * TicketVoteSelector.h
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_TICKETVOTESELECTOR_H_
#define BC_FINALIZER_TICKETVOTESELECTOR_H_

#include "base/ArrayList.h"

#include "bc_finalizer/VoterEntry.h"

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class VoteTicket;
class CodablecashConfig;

class TicketVoteSelector {
public:
	explicit TicketVoteSelector(ArrayList<VoterEntry, VoterEntry::VoteCompare>* list, uint64_t height, const CodablecashConfig* config);
	virtual ~TicketVoteSelector();

	void select();

	const ArrayList<const VoteTicket>* getSelectedList() const noexcept {
		return this->selected;
	}

private:
	void doSelect();
	const VoteTicket* selectFromList(ArrayList<const VoteTicket>* list, int count, ByteBuffer* shabuff);
	ByteBuffer* makebuffer();

	void resetPosition();
	void makeList();
	void addList(const VoteTicket* ticket);

private:
	ArrayList<VoterEntry, VoterEntry::VoteCompare>* list;
	uint64_t height;

	ArrayList<const VoteTicket>* candidateList;
	ArrayList<const VoteTicket>* expiredList;
	ArrayList<const VoteTicket>* selected;

	uint16_t tiketMatureIntervalHeight;
	uint64_t ticketExpireHeight;

	int votePerBlock;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_TICKETVOTESELECTOR_H_ */
