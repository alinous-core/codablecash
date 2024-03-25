/*
 * NodeLiveData.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NETWORK_NODELIVEDATA_H_
#define BC_P2P_CMD_NETWORK_NODELIVEDATA_H_

#include "data_history/AbstractTransferedData.h"

namespace codablecash {

class NodeLiveData : public AbstractTransferedData {
public:
	NodeLiveData(const NodeLiveData& inst);
	NodeLiveData();
	virtual ~NodeLiveData();
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NETWORK_NODELIVEDATA_H_ */
