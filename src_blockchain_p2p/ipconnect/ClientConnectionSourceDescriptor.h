/*
 * ClientConnectionSourceDescriptor.h
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#ifndef IPCONNECT_CLIENTCONNECTIONSOURCEDESCRIPTOR_H_
#define IPCONNECT_CLIENTCONNECTIONSOURCEDESCRIPTOR_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class ClientConnectionSourceDescriptor {
public:
	static const constexpr uint8_t IPV4{1};
	static const constexpr uint8_t IPV6{2};

	ClientConnectionSourceDescriptor(const ClientConnectionSourceDescriptor& inst);
	ClientConnectionSourceDescriptor(uint8_t family, const UnicodeString* host, int port);
	virtual ~ClientConnectionSourceDescriptor();

private:
	uint8_t afFamily;
	UnicodeString* host;
	int port;
};

} /* namespace codablecash */

#endif /* IPCONNECT_CLIENTCONNECTIONSOURCEDESCRIPTOR_H_ */
