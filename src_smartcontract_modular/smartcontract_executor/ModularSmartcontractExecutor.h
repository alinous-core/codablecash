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
class SmartcontractProjectId;
class SmartcontractProjectData;
class InstanceSpacesManager;
class InstanceSpace;
class SmartcontractInstanceAddress;

class ModularSmartcontractExecutor {
public:
	static const constexpr wchar_t* DIR_NAME{L"contract_exec"};
	static const constexpr wchar_t* INSTANCES_DIR_NAME{L"instances"};

	explicit ModularSmartcontractExecutor(const File* base);
	virtual ~ModularSmartcontractExecutor();

	void createExecutor();
	void open();
	void close() noexcept;

	void registerModularSmartcontractProject(ModularSmartcontractProject* project);
	SmartcontractProjectData* getProject(const SmartcontractProjectId* projectId);

	void createInstance(const SmartcontractInstanceAddress* instAddress, const SmartcontractProjectId* projectId);
	InstanceSpace* loadFromCache(const SmartcontractInstanceAddress* instAddress);

private:
	File* baseDir;
	ModularSmartcontractProjectRegistory* projectRegistory;
	SmartcontractExecContextRegistory* execContextRegistory;
	InstanceIdIndex* instanceIndex;

	InstanceSpacesManager* instanceSpace;

};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_MODULARSMARTCONTRACTEXECUTOR_H_ */
