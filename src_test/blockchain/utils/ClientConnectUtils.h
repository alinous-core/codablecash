/*
 * ClientConnectUtils.h
 *
 *  Created on: 2023/09/28
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_CLIENTCONNECTUTILS_H_
#define BLOCKCHAIN_UTILS_CLIENTCONNECTUTILS_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class P2pClient;
class ISystemLogger;

class ClientConnectUtils {
public:
	static P2pClient* connect(const UnicodeString* host, int port, uint16_t zone, ISystemLogger* logger);
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_CLIENTCONNECTUTILS_H_ */
