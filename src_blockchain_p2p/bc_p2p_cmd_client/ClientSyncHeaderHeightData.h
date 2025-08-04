/*
 * ClientSyncHeaderHeightData.h
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERHEIGHTDATA_H_
#define BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERHEIGHTDATA_H_
#include <cstdint>

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class ClientBlockHeaderTransferData;

class ClientSyncHeaderHeightData : public IBlockObject {
public:
	ClientSyncHeaderHeightData(const ClientSyncHeaderHeightData& inst);
	ClientSyncHeaderHeightData();
	virtual ~ClientSyncHeaderHeightData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static ClientSyncHeaderHeightData* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	UnicodeString* toString() const noexcept;

	uint64_t getHeight() const noexcept {
		return this->height;
	}
	void addHeaderData(const ClientBlockHeaderTransferData* data);

	void verifyVoteTransactionsData() const;
	void verifyTransactionsData() const;

	ArrayList<ClientBlockHeaderTransferData>* getDataList() const noexcept {
		return this->list;
	}
	void setHeight(uint64_t height) noexcept;

private:
	uint64_t height;
	ArrayList<ClientBlockHeaderTransferData>* list;

};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTSYNCHEADERHEIGHTDATA_H_ */
