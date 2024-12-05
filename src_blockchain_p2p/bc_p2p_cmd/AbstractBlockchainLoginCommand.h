/*
 * AbstractBlockchainLoginCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_ABSTRACTBLOCKCHAINLOGINCOMMAND_H_
#define BC_P2P_CMD_ABSTRACTBLOCKCHAINLOGINCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainPubsubCommand.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractBlockchainLoginCommand : public AbstractBlockchainPubsubCommand {
public:
	AbstractBlockchainLoginCommand(const AbstractBlockchainLoginCommand& inst);
	AbstractBlockchainLoginCommand(uint32_t type, uint16_t zone, const UnicodeString* canonicalName);
	virtual ~AbstractBlockchainLoginCommand();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	uint16_t getZone() const noexcept {
		return this->zone;
	}
	const UnicodeString* getCanonicalName() const noexcept {
		return this->canonicalName;
	}

protected:
	uint16_t zone;
	UnicodeString* canonicalName;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_ABSTRACTBLOCKCHAINLOGINCOMMAND_H_ */
