/*
 * PoWStartCalculateCommandMessage.h
 *
 *  Created on: 2023/03/29
 *      Author: iizuka
 */

#ifndef POW_POWSTARTCALCULATECOMMANDMESSAGE_H_
#define POW_POWSTARTCALCULATECOMMANDMESSAGE_H_

#include "pow/AbstractPoWCommandMessage.h"

#include <cstdint>

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderId;

class PoWStartCalculateCommandMessage : public AbstractPoWCommandMessage {
public:
	explicit PoWStartCalculateCommandMessage();
	virtual ~PoWStartCalculateCommandMessage();

protected:
	virtual void process(PoWManager* manager);

};

} /* namespace codablecash */

#endif /* POW_POWSTARTCALCULATECOMMANDMESSAGE_H_ */
