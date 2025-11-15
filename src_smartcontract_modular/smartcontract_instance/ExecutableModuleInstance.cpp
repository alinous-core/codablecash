/*
 * ExecutableModule.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */
#include "smartcontract_instance/ExecutableModuleInstance.h"

#include "base_io/File.h"

#include "base_io_stream/exceptions.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc/ExceptionThrower.h"


namespace codablecash {

ExecutableModuleInstance::ExecutableModuleInstance() {


}

ExecutableModuleInstance::~ExecutableModuleInstance() {

}

void ExecutableModuleInstance::loadCompilantUnits(const File *projectBaseDir) {
	File* baseDir = projectBaseDir->get(this->projectRelativePath); __STP(baseDir);

	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->exists(), L"Module base folder does not exists.", __FILE__, __LINE__);
	ExceptionThrower<FileIOException>::throwExceptionIfCondition(!baseDir->isDirectory(), L"Module base folder id not a directory.", __FILE__, __LINE__);




}

} /* namespace codablecash */
