/*
 * PendingCommandData.h
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATA_H_
#define BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class PubSubId;
class AbstractNodeCommand;

class PendingCommandData : public IBlockObject {
public:
	PendingCommandData(const PendingCommandData& inst);
	PendingCommandData();
	virtual ~PendingCommandData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static PendingCommandData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setPubsubId(const PubSubId* pubsubId) noexcept;
	void setNodeCommand(const AbstractNodeCommand* command) noexcept;

	const PubSubId* getPubSubId() const noexcept {
		return this->pubsubId;
	}
	const AbstractNodeCommand* getNodeCommand() const noexcept {
		return this->command;
	}

private:
	PubSubId* pubsubId;
	AbstractNodeCommand* command;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_PENDING_PENDINGCOMMANDDATA_H_ */
