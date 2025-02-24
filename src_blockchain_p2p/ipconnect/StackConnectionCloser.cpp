/*
 * StackConnectionCloser.cpp
 *
 *  Created on: Feb 2, 2025
 *      Author: iizuka
 */

#include "ipconnect/StackConnectionCloser.h"
#include "ipconnect/IClientSocket.h"

namespace codablecash {

StackConnectionCloser::StackConnectionCloser(IClientSocket* con) {
	this->con = con;

}

StackConnectionCloser::~StackConnectionCloser() {
	if(this->con != nullptr){
		this->con->close();
	}
}

} /* namespace codablecash */
