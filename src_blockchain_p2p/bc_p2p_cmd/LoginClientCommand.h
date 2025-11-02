/*
 * LoginClientCommand.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_LOGINCLIENTCOMMAND_H_
#define BC_P2P_CMD_LOGINCLIENTCOMMAND_H_

#include "bc_p2p_cmd/AbstractBlockchainLoginCommand.h"

#include "base/ArrayList.h"


namespace codablecash {

class BloomFilter1024;

class LoginClientCommand : public AbstractBlockchainLoginCommand {
public:
	LoginClientCommand(const LoginClientCommand& inst);
	explicit LoginClientCommand(uint16_t zone);
	virtual ~LoginClientCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;

	virtual IBlockObject* copyData() const noexcept;

	void addBloomFilter(const BloomFilter1024* f);
	const ArrayList<BloomFilter1024>* getFilterList() const noexcept {
		return this->filterList;
	}

protected:
	virtual ByteBuffer* getSignBinary() const;

protected:
	ArrayList<BloomFilter1024>* filterList;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_LOGINCLIENTCOMMAND_H_ */
