/*
 * ValidationInstance.h
 *
 *  Created on: 2023/12/03
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_VALIDATIONINSTANCE_H_
#define BLOCKCHAIN_UTILS_VALIDATIONINSTANCE_H_
#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class CodablecashNodeInstance;
class CodablecashConfig;
class ISystemLogger;
class Block;

class ValidationInstance {
public:
	explicit ValidationInstance(const File* baseDir);
	virtual ~ValidationInstance();

	void initInstance(const CodablecashConfig* config, ISystemLogger* logger);
	void validateBlock(const Block* block);

private:
	File* instDir;
	CodablecashNodeInstance* inst;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_VALIDATIONINSTANCE_H_ */
