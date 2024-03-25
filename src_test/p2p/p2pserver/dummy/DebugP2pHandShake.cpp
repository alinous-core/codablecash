/*
 * DebugP2pHandShake.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */

#include "DebugP2pHandShake.h"

namespace codablecash {

DebugP2pHandShake::DebugP2pHandShake(const PubSubId* psId, ISystemLogger* logger)
		: P2pHandshake(psId, logger) {
}

DebugP2pHandShake::~DebugP2pHandShake() {

}

} /* namespace codablecash */
