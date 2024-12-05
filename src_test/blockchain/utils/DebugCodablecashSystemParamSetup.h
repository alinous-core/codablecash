/*
 * DebugCodablecashConfigSetup.h
 *
 *  Created on: 2023/05/04
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_DEBUGCODABLECASHSYSTEMPARAMSETUP_H_
#define BLOCKCHAIN_UTILS_DEBUGCODABLECASHSYSTEMPARAMSETUP_H_

namespace codablecash {

class CodablecashSystemParam;

class DebugCodablecashSystemParamSetup {
public:
	static void setupConfig01(CodablecashSystemParam& param);

	static void setupConfig02(CodablecashSystemParam& param); // test net
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_DEBUGCODABLECASHSYSTEMPARAMSETUP_H_ */
