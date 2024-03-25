/*
 * AbstractConnectionCommand.h
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#ifndef P2PSERVER_ABSTRACTCONNECTIONCOMMAND_H_
#define P2PSERVER_ABSTRACTCONNECTIONCOMMAND_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class IClientSocket;
class P2pConnectionAcceptThread;

class AbstractConnectionCommand {
public:
	static const constexpr uint8_t TYPE_NOP{1};
	static const constexpr uint8_t TYPE_CONNECT_SUBSCRIBER{11};
	static const constexpr uint8_t TYPE_CONNECT_PUBLISHER{12};

	AbstractConnectionCommand(uint8_t type);
	virtual ~AbstractConnectionCommand();

	static AbstractConnectionCommand* createFromBinary(ByteBuffer* buff);

	virtual void fromBinary(ByteBuffer* buff) = 0;
	virtual int binarySize() = 0;
	virtual void toBinary(ByteBuffer* buff) = 0;

	virtual void process(P2pConnectionAcceptThread* acceptThread) = 0;

	virtual void sendCommand(IClientSocket* client);
	virtual void reveiveResponse(IClientSocket* client);
	virtual bool isOk();

protected:
	void sendResponseString(IClientSocket *client, const char* str, int size);
	void sendOkString(IClientSocket *client);

private:
	ByteBuffer* readAll(IClientSocket *client, int size);

protected:
	uint8_t type;
	ByteBuffer* responseBinary;
};

} /* namespace codablecash */

#endif /* P2PSERVER_ABSTRACTCONNECTIONCOMMAND_H_ */
