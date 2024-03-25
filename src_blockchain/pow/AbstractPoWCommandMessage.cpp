/*
 * AbstractPoWCommandMessage.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "pow/AbstractPoWCommandMessage.h"

#include "pow/PoWManager.h"

namespace codablecash {

AbstractPoWCommandMessage::AbstractPoWCommandMessage() {

}

AbstractPoWCommandMessage::~AbstractPoWCommandMessage() {

}

void AbstractPoWCommandMessage::execute(ICommandParameter *param) {
	PoWManager* manager = dynamic_cast<PoWManager*>(param);

	process(manager);
}

} /* namespace codablecash */
