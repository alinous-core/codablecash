/*
 * DummyClientListner.cpp
 *
 *  Created on: 2023/10/11
 *      Author: iizuka
 */

#include "DummyClientListner.h"

#include "data_history_data/TransactionTransferData.h"

#include "osenv/funcs.h"
namespace codablecash {

DummyClientListner::DummyClientListner() {
	this->list = new ArrayList<TransactionTransferData>();
}

DummyClientListner::~DummyClientListner() {
	this->list->deleteElements();
	delete this->list;
}

void DummyClientListner::onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData *data) {
	TransactionTransferData* trxdata = new TransactionTransferData(*data);
	this->list->addElement(trxdata);
}

int DummyClientListner::size() const noexcept {
	return this->list->size();
}

void DummyClientListner::waitUntilCount(int trxCount) {
	int num = size();

	while(num < trxCount){
		Os::usleep(10*1000);
		num = size();
	}
}

void DummyClientListner::onBlockMined(const PubSubId *pubsubId, const BlockHeaderTransferData *blockcheader) {
}

} /* namespace codablecash */
