/*
 * RamdomNodesSelector.h
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#ifndef BC_NETWORK_INSTANCE_SYNC_RAMDOMNODESSELECTOR_H_
#define BC_NETWORK_INSTANCE_SYNC_RAMDOMNODESSELECTOR_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifier;

class RamdomNodesSelector {
public:
	RamdomNodesSelector(const ArrayList<NodeIdentifier>* nodelist, int numSelect);
	virtual ~RamdomNodesSelector();

	bool hasNext() const noexcept;
	const NodeIdentifier* next() noexcept;

private:
	ArrayList<const NodeIdentifier>* list;
	int numSelect;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_INSTANCE_SYNC_RAMDOMNODESSELECTOR_H_ */
