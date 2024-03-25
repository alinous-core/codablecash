/*
 * NopCommandMessage.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "procesor/NopCommandMessage.h"
#include "procesor/BlankCommandParameter.h"

#include "procesor/ICommandParameter.h"

namespace codablecash {

NopCommandMessage::NopCommandMessage() {

}

NopCommandMessage::~NopCommandMessage() {

}

void NopCommandMessage::execute(ICommandParameter *param) {
	BlankCommandParameter* bkparam = dynamic_cast<BlankCommandParameter*>(param);
	if(bkparam != nullptr){
		bkparam->incCount();
	}
}
} /* namespace codablecash */
