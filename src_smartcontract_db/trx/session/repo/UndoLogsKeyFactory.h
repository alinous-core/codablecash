/*
 * UndoLogsKeyFactory.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSKEYFACTORY_H_
#define TRX_SESSION_REPO_UNDOLOGSKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class UndoLogsKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t UNDO_LOGS_ID_KEY{0x10};
	static const constexpr uint32_t UNDO_LOGS_PART_ID_KEY{0x11};
	static const constexpr uint32_t UNDO_LOGS_SESSION_ID_KEY{0x12};

	UndoLogsKeyFactory();
	virtual ~UndoLogsKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSKEYFACTORY_H_ */
