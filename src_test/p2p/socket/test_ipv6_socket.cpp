/*
 * test_ipv6_socket.cpp
 *
 *  Created on: 2022/02/11
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "osenv/sockets.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "base/StackRelease.h"

#include "../../test_utils/TestPortSelection.h"
#include "base/Integer.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(IpV6TTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(IpV6TTestGroup, case01){
	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	UnicodeString* portStr = Integer::toString(port, 10); __STP(portStr);

	IPV6Context context = IPV6::socket(nullptr, portStr);

	CHECK(context.sockId > 0)

	IPV6::close(&context);

	IPV6Context context2 = context;
}

TEST(IpV6TTestGroup, case02){
	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	UnicodeString* portStr = Integer::toString(port, 10); __STP(portStr);

	IPV6Context context = IPV6::socket(nullptr, portStr);
	context.release();

	IPV6::close(&context);
}

TEST(IpV6TTestGroup, case03){
	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	UnicodeString* portStr = Integer::toString(port, 10); __STP(portStr);

	IPV6Context context = IPV6::socket(nullptr, portStr, -1);

	IPV6::close(&context);
}
TEST(IpV6TTestGroup, case04){
	IpV6ClientConnection* con = new IpV6ClientConnection(); __STP(con);
	IClientSocket* con2 = con->newSameClient(); __STP(con2);

	CHECK(dynamic_cast<IpV6ClientConnection*>(con2) != nullptr);
}
