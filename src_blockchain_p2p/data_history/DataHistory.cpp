/*
 * DataHistory.cpp
 *
 *  Created on: 2023/09/28
 *      Author: iizuka
 */

#include "data_history/DataHistory.h"
#include "data_history/DataHistoryStore.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

namespace codablecash {

DataHistory::DataHistory(const File* baseDir) {
	this->mutex = new SysMutex();
	this->store = new DataHistoryStore(baseDir);
}

DataHistory::~DataHistory() {
	delete this->store;
	delete this->mutex;
}

void DataHistory::createBlankDatabase() {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	this->store->createBlankDatabase();
}

void DataHistory::open() {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	this->store->open();
}

void DataHistory::close() {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	this->store->close();
}

void DataHistory::add(const TransferedDataId *dataId, const AbstractTransferedData* data) {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	this->store->add(dataId, data);
}

bool DataHistory::hasId(const TransferedDataId *dataId) const {
	StackUnlocker __unlock(this->mutex, __FILE__, __LINE__);

	return this->store->hasId(dataId);
}

} /* namespace codablecash */
