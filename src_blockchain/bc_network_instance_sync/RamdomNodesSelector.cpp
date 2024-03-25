/*
 * RamdomNodesSelector.cpp
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#include "bc_network_instance_sync/RamdomNodesSelector.h"

#include "bc_network/NodeIdentifier.h"

#include "osenv/funcs.h"


namespace codablecash {

RamdomNodesSelector::RamdomNodesSelector(const ArrayList<NodeIdentifier>* nodelist, int numSelect) {
	this->numSelect = numSelect;
	this->list = new ArrayList<const NodeIdentifier>();

	int maxLoop = nodelist->size();
	for(int i = 0; i != maxLoop; ++i){
		NodeIdentifier* nodeId = nodelist->get(i);

		this->list->addElement(nodeId);
	}
}

RamdomNodesSelector::~RamdomNodesSelector() {
	delete this->list;
}

bool RamdomNodesSelector::hasNext() const noexcept {
	return !this->list->isEmpty() && this->numSelect > 0;
}

const NodeIdentifier* RamdomNodesSelector::next() noexcept {
	uint64_t tm = Os::getMicroSec();
	int index = tm % this->list->size();

	this->numSelect--;

	return this->list->remove(index);
}

} /* namespace codablecash */
