/*
 * BlankCommandParameter.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef PROCESOR_BLANKCOMMANDPARAMETER_H_
#define PROCESOR_BLANKCOMMANDPARAMETER_H_

namespace alinous {
class SysMutex;
}
using namespace alinous;

#include "procesor/ICommandParameter.h"

namespace codablecash {

class BlankCommandParameter : public ICommandParameter {
public:
	BlankCommandParameter();
	virtual ~BlankCommandParameter();

	int getCount() const noexcept {
		return count;
	}
	void incCount() noexcept;

private:
	int count;
	SysMutex* mutex;
};

} /* namespace codablecash */

#endif /* PROCESOR_BLANKCOMMANDPARAMETER_H_ */
