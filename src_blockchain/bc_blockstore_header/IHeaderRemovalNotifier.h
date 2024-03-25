/*
 * IHeaderRemovalNotifier.h
 *
 *  Created on: 2023/05/11
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_IHEADERREMOVALNOTIFIER_H_
#define BC_BLOCKSTORE_HEADER_IHEADERREMOVALNOTIFIER_H_

namespace codablecash {

class BlockHeader;

class IHeaderRemovalNotifier {
public:
	IHeaderRemovalNotifier();
	virtual ~IHeaderRemovalNotifier();

	virtual void onRemovedHeader(const BlockHeader* header) noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_IHEADERREMOVALNOTIFIER_H_ */
