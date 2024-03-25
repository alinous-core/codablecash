/*
 * FileSnapshotRecoverer.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/FileSnapshotRecoverer.h"
#include "snapshot/AbstractSnapshotCommand.h"
#include "snapshot/SchemaStartSnapshotCommand.h"
#include "snapshot/TableStartSnapshotCommand.h"

#include "base_io_stream/FileInputStream.h"

#include "base_io/File.h"
#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"


using namespace alinous;

namespace codablecash {

FileSnapshotRecoverer::FileSnapshotRecoverer() {
	this->inStream = nullptr;
	this->stack = new ArrayList<AbstractSnapshotCommand>();
}

FileSnapshotRecoverer::~FileSnapshotRecoverer() {
	close();

	this->stack->deleteElements();
	delete this->stack;
}

void FileSnapshotRecoverer::open(const File *file) {
	this->inStream = new FileInputStream(file);
	this->inStream->open();
}

void FileSnapshotRecoverer::close() noexcept {
	if(this->inStream != nullptr){
		this->inStream->close();
		delete this->inStream, this->inStream = nullptr;
	}
}

void FileSnapshotRecoverer::recover(CodableDatabase* db) {
	uint16_t size = readShort();

	while(size > 0){
		AbstractSnapshotCommand* command = readCommand(size); __STP(command);

		command->recover(this, db);

		if(command->popStack()){
			delete popStack();
			}
		else if(command->pushStack()){
			pushStack(__STP_MV(command));
		}

		size = readShort();
	}
}

AbstractSnapshotCommand* FileSnapshotRecoverer::readCommand(uint16_t size) {
	char* data = new char[size];
	StackArrayRelease<char> __data(data);

	int n = this->inStream->read(data, size);

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(data, n, true); __STP(buff);

	return AbstractSnapshotCommand::createFromBinary(buff);
}

uint16_t FileSnapshotRecoverer::readShort() {
	char tmp[2];
	int read = this->inStream->read(tmp, 2);
	if(read <= 0){
		return 0;
	}

	ByteBuffer* buff = ByteBuffer::wrapWithEndian(tmp, 2, true); __STP(buff);
	buff->position(0);

	return buff->getShort();
}

void FileSnapshotRecoverer::pushStack(AbstractSnapshotCommand *cmd) noexcept {
	this->stack->addElement(cmd);
}

SchemaStartSnapshotCommand* FileSnapshotRecoverer::getSchemaStartSnapshotCommand() const {
	AbstractSnapshotCommand* cmd = findCommand(AbstractSnapshotCommand::SCHEMA_START);

	return dynamic_cast<SchemaStartSnapshotCommand*>(cmd);
}

TableStartSnapshotCommand* FileSnapshotRecoverer::getTableStartSnapshotCommand() const {
	AbstractSnapshotCommand* cmd = findCommand(AbstractSnapshotCommand::TABLE_START);

	return dynamic_cast<TableStartSnapshotCommand*>(cmd);
}

AbstractSnapshotCommand* FileSnapshotRecoverer::findCommand(uint8_t type) const {
	AbstractSnapshotCommand* ret = nullptr;

	int index = this->stack->size() - 1;
	for(int i = index; i >= 0; --i){
		AbstractSnapshotCommand* cmd = this->stack->get(i);

		if(cmd->getType() == type){
			ret = cmd;
		}
	}

	return ret;
}

AbstractSnapshotCommand* FileSnapshotRecoverer::popStack() noexcept {
	int idx = this->stack->size() - 1;
	return this->stack->remove(idx);
}

} /* namespace codablecash */
