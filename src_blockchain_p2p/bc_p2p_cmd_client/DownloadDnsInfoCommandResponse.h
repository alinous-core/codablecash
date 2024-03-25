/*
 * DownloadDnsInfoCommandResponse.h
 *
 *  Created on: 2023/10/16
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/ArrayList.h"

namespace codablecash {

class P2pNodeRecord;

class DownloadDnsInfoCommandResponse : public AbstractCommandResponse {
public:
	DownloadDnsInfoCommandResponse();
	virtual ~DownloadDnsInfoCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;
	virtual UnicodeString* toString() const noexcept;

	void importRecords(const ArrayList<P2pNodeRecord>* records);
	const ArrayList<P2pNodeRecord>* getRecords() const noexcept {
		return this->list;
	}

private:
	ArrayList<P2pNodeRecord>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_DOWNLOADDNSINFOCOMMANDRESPONSE_H_ */
