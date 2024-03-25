/*
 * PoWGenerateBlockRequestCommandMessage.h
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#ifndef POW_POWGENERATEBLOCKREQUESTCOMMANDMESSAGE_H_
#define POW_POWGENERATEBLOCKREQUESTCOMMANDMESSAGE_H_

#include "pow/AbstractPoWCommandMessage.h"

#include <cstdint>

namespace codablecash {

class BlockHeaderId;
class PoWNonce;

class PoWGenerateBlockRequestCommandMessage : public AbstractPoWCommandMessage {
public:
	PoWGenerateBlockRequestCommandMessage(uint64_t lastBlockHeight, const BlockHeaderId *lastBlockHeaderId, const PoWNonce *nonce);
	virtual ~PoWGenerateBlockRequestCommandMessage();

protected:
	virtual void process(PoWManager* manager);


private:
	uint64_t lastBlockHeight;
	BlockHeaderId *lastBlockHeaderId;
	PoWNonce *nonce;
};

} /* namespace codablecash */

#endif /* POW_POWGENERATEBLOCKREQUESTCOMMANDMESSAGE_H_ */
