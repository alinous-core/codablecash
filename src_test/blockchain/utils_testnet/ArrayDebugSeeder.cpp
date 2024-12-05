/*
 * ArrayDebugSeeder.cpp
 *
 *  Created on: Dec 4, 2024
 *      Author: iizuka
 */

#include "ArrayDebugSeeder.h"

#include "bc_p2p_info/P2pNodeRecord.h"


namespace codablecash {

ArrayDebugSeeder::ArrayDebugSeeder() {
	this->list = new ArrayList<P2pNodeRecord>();
}

ArrayDebugSeeder::~ArrayDebugSeeder() {
	this->list->deleteElements();
	delete this->list;
}

ArrayList<P2pNodeRecord>* ArrayDebugSeeder::getSeedNodes() const noexcept {
	ArrayList<P2pNodeRecord>* ret = new ArrayList<P2pNodeRecord>();

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		const P2pNodeRecord* record = this->list->get(i);

		ret->addElement(dynamic_cast<P2pNodeRecord*>(record->copyData()));
	}

	return ret;
}

void ArrayDebugSeeder::addRecord(const P2pNodeRecord *record) noexcept {
	this->list->addElement(dynamic_cast<P2pNodeRecord*>(record->copyData()));
}

} /* namespace codablecash */
