/*
 * NodeLiveData.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "bc_p2p_cmd_network/NodeLiveData.h"

namespace codablecash {

NodeLiveData::NodeLiveData(const NodeLiveData &inst) : AbstractTransferedData(inst) {
}

NodeLiveData::NodeLiveData() : AbstractTransferedData(DATA_LIVEDATA) {
	// TODO Auto-generated constructor stub

}

NodeLiveData::~NodeLiveData() {

}

} /* namespace codablecash */
