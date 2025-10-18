/*
 * ModuleConfigException.h
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */

#ifndef MODULAR_PROJECT_MODULARCONFIGEXCEPTION_H_
#define MODULAR_PROJECT_MODULARCONFIGEXCEPTION_H_

#include "base/Exception.h"

using namespace alinous;

namespace codablecash {

class ModularConfigException : public alinous::Exception {
public:
	ModularConfigException(const char* srcfile, int srcline) noexcept;
	ModularConfigException(Exception* cause, const char* srcfile, int srcline) noexcept;
	ModularConfigException(const wchar_t* message, const char* srcfile, int srcline) noexcept;
	ModularConfigException(const wchar_t* message, Exception* cause, const char* srcfile, int srcline) noexcept;
	virtual ~ModularConfigException();
	static const wchar_t* defaultMessage;
};

} /* namespace codablecash */

#endif /* MODULAR_PROJECT_MODULARCONFIGEXCEPTION_H_ */
