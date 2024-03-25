/*
 * BtreeReverseScanner.cpp
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#include "btree/BtreeReverseScanner.h"
#include "btree/AbstractBtreeKey.h"
#include "btree/NodeCursor.h"

#include "filestore_block/IBlockObject.h"

namespace alinous {

BtreeReverseScanner::BtreeReverseScanner(NodeCursor* cursor) {
	this->cursor = cursor;
	this->nextObj = nullptr;
	this->initialized = false;
	this->key = nullptr;
}

BtreeReverseScanner::~BtreeReverseScanner() {
	delete this->cursor;
	delete this->nextObj;
	delete this->key;
}

void BtreeReverseScanner::begin() {
}

void BtreeReverseScanner::begin(const AbstractBtreeKey *key) {
	this->key = key != nullptr ? key->clone() : nullptr;
}

bool BtreeReverseScanner::hasPrevious() {
	if(this->nextObj != nullptr){
		delete this->nextObj, this->nextObj = nullptr;
	}

	if(!this->initialized){
		if(this->key == nullptr){
			this->nextObj = this->cursor->gotoLast();
			this->initialized = true;
		}
		else{
			this->nextObj = this->cursor->gotoKeyPrevious(this->key);
			this->initialized = true;
		}
	}
	else{
		this->nextObj = this->cursor->getPrevious();
	}

	return this->nextObj != nullptr;
}

const IBlockObject* BtreeReverseScanner::previous() {
	return this->nextObj;
}

const AbstractBtreeKey* BtreeReverseScanner::previousKey() {
	return this->cursor->getCurrentKey();
}

} /* namespace alinous */
