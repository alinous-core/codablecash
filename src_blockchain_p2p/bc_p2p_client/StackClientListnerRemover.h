/*
 * StackClientListnerRemover.h
 *
 *  Created on: 2023/11/13
 *      Author: iizuka
 */

#ifndef BC_P2P_CLIENT_STACKCLIENTLISTNERREMOVER_H_
#define BC_P2P_CLIENT_STACKCLIENTLISTNERREMOVER_H_

namespace codablecash {

class IClientNotifyListner;
class P2pClient;

class StackClientListnerRemover {
public:
	StackClientListnerRemover(P2pClient* client, IClientNotifyListner* listner);
	virtual ~StackClientListnerRemover();

private:
	P2pClient* client;
	IClientNotifyListner* listner;
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENT_STACKCLIENTLISTNERREMOVER_H_ */
