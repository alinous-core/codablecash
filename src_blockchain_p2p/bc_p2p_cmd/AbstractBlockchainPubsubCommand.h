/*
 * AbstractBlockchainPubsubCommand.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_ABSTRACTBLOCKCHAINPUBSUBCOMMAND_H_
#define BC_P2P_CMD_ABSTRACTBLOCKCHAINPUBSUBCOMMAND_H_

#include "pubsub_cmd/AbstractPubSubCommand.h"

namespace alinous {
class BigInteger;
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class SchnorrSignature;
class NodeIdentifier;
class NodeIdentifierSource;

class AbstractBlockchainPubsubCommand : public AbstractPubSubCommand {
public:
	AbstractBlockchainPubsubCommand(const AbstractBlockchainPubsubCommand& inst);
	explicit AbstractBlockchainPubsubCommand(uint32_t type);
	virtual ~AbstractBlockchainPubsubCommand();

	void sign(const NodeIdentifierSource* source);
	bool verify() const;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

	void setNodeIdentifier(const NodeIdentifier* nodeId) noexcept;
	const NodeIdentifier* getNodeId() const noexcept {
		return this->nodeId;
	}

protected:
	void checkSignature() const;

	virtual ByteBuffer* getSignBinary() const = 0;

	int sigBinarySize() const;
	void toSigBinary(ByteBuffer* out) const;
	void fromSigBinary(ByteBuffer* in);

protected:
	NodeIdentifier* nodeId;
	SchnorrSignature* sig;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_ABSTRACTBLOCKCHAINPUBSUBCOMMAND_H_ */
