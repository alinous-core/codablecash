/*
 * DummyCommand.cpp
 *
 *  Created on: 2023/08/15
 *      Author: iizuka
 */

#include "DummyErrorNopCommand.h"

#include "base_io_stream/exceptions.h"

using alinous::FileIOException;

namespace codablecash {

DummyErrorNopCommand::DummyErrorNopCommand() {

}

DummyErrorNopCommand::~DummyErrorNopCommand() {

}

void DummyErrorNopCommand::execute(ICommandParameter *param) {
	throw new FileIOException(__FILE__, __LINE__);
}

} /* namespace codablecash */
