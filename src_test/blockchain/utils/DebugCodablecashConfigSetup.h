/*
 * DebugCodablecashConfigSetup.h
 *
 *  Created on: 2023/05/04
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_DEBUGCODABLECASHCONFIGSETUP_H_
#define BLOCKCHAIN_UTILS_DEBUGCODABLECASHCONFIGSETUP_H_

namespace codablecash {

class CodablecashConfig;

class DebugCodablecashConfigSetup {
public:
	static void setupConfig01(CodablecashConfig& config);

	static void setupConfig02(CodablecashConfig& config); // test net
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_DEBUGCODABLECASHCONFIGSETUP_H_ */
