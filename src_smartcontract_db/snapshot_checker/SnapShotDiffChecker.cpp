/*
 * SnapShotDiffChecker.cpp
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */
#include "snapshot_checker/SnapShotDiffChecker.h"
#include "snapshot_checker/SnapshotFileReader.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"

#include "base/StackRelease.h"

#include "snapshot/AbstractSnapshotCommand.h"
namespace codablecash {

SnapShotDiffChecker::SnapShotDiffChecker(const File* a, const File* b) {
	this->readerA = new SnapshotFileReader(a);
	this->readerB = new SnapshotFileReader(b);
}

SnapShotDiffChecker::~SnapShotDiffChecker() {
	delete this->readerA;
	delete this->readerB;
}

void SnapShotDiffChecker::test() {
	this->readerA->init();
	this->readerB->init();

	uint16_t size = this->readerA->readShort();
	uint16_t sizeB = this->readerB->readShort();
	DatabaseEqualsChecker::checkIntEquals(size, sizeB);

	while(size > 0){
		AbstractSnapshotCommand* command = this->readerA->readCommand(size); __STP(command);
		AbstractSnapshotCommand* commandB = this->readerB->readCommand(sizeB); __STP(commandB);

		command->__testCheckEquals(commandB);

		size = this->readerA->readShort();
		sizeB = this->readerB->readShort();
		DatabaseEqualsChecker::checkIntEquals(size, sizeB);
	}
}

} /* namespace codablecash */
