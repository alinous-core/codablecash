/*
 * ModularSmartcontractExecutor.h
 *
 *  Created on: Sep 22, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_EXECUTOR_MODULARSMARTCONTRACTEXECUTOR_H_
#define SMARTCONTRACT_EXECUTOR_MODULARSMARTCONTRACTEXECUTOR_H_

namespace alinous {
class UnicodeString;
class File;
}
using namespace alinous;

namespace codablecash {

class ModularSmartcontractProjectRegistory;
class SmartcontractExecContextRegistory;
class InstanceIdIndex;
class ModularSmartcontractProject;

class ModularSmartcontractExecutor {
public:
	static const constexpr wchar_t* DIR_NAME{L"contract_exec"};

	explicit ModularSmartcontractExecutor(const File* base);
	virtual ~ModularSmartcontractExecutor();

	void registerModularSmartcontractProject(ModularSmartcontractProject* project);

private:
	File* baseDir;
	ModularSmartcontractProjectRegistory* projectRegistory;
	SmartcontractExecContextRegistory* execContextRegistory;
	InstanceIdIndex* instanceIndex;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_MODULARSMARTCONTRACTEXECUTOR_H_ */
