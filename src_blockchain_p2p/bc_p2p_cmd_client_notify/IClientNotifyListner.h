/*
 * IClientNotifyListner.h
 *
 *  Created on: 2023/10/05
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_NOTIFY_ICLIENTNOTIFYLISTNER_H_
#define BC_P2P_CMD_CLIENT_NOTIFY_ICLIENTNOTIFYLISTNER_H_

namespace codablecash {

class PubSubId;
class TransactionTransferData;
class BlockHeaderTransferData;

class IClientNotifyListner {
public:
	IClientNotifyListner();
	virtual ~IClientNotifyListner();

	virtual void onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData* data) = 0;
	virtual void onBlockMined(const PubSubId *pubsubId, const BlockHeaderTransferData *blockcheaderData) = 0;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_NOTIFY_ICLIENTNOTIFYLISTNER_H_ */
