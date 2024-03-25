/*
 * DownloadDnsInfoCommand.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMAND_H_
#define BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMAND_H_

#include "bc_p2p_cmd_client/AbstractClientRequestCommand.h"

namespace codablecash {

class DownloadDnsInfoCommand : public AbstractClientRequestCommand {
public:
	static const constexpr int MAX_DL{1024};

	DownloadDnsInfoCommand(const DownloadDnsInfoCommand& inst);
	DownloadDnsInfoCommand(uint16_t zone);
	virtual ~DownloadDnsInfoCommand();

	virtual IBlockObject* copyData() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual void fromBinary(ByteBuffer* buff);

protected:
	virtual AbstractCommandResponse* executeAsClient(ClientNodeHandshake* clientHandshake, CodablecashNodeInstance* inst) const;

	virtual ByteBuffer* getSignBinary() const;

private:
	uint16_t zone;
	uint16_t maxNum;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMAND_H_ */
