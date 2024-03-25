/*
 * DataStructureException.h
 *
 *  Created on: 2023/11/09
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_DATA_DATASTRUCTUREEXCEPTION_H_
#define DATA_HISTORY_DATA_DATASTRUCTUREEXCEPTION_H_

#include "base/Exception.h"


using namespace alinous;

namespace codablecash {

class DataStructureException : public Exception {
public:
	DataStructureException(const char* srcfile, int srcline) noexcept;
	DataStructureException(Exception* cause, const char* srcfile, int srcline) noexcept;
	DataStructureException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	DataStructureException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~DataStructureException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_DATA_DATASTRUCTUREEXCEPTION_H_ */
