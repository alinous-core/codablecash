/*
 * ExceptionThrower.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_EXCEPTIONTHROWER_H_
#define BC_EXCEPTIONTHROWER_H_

namespace codablecash {

template <class E>
class ExceptionThrower {
public:
	static void throwExceptionIfCondition(bool condition, const wchar_t* message, const char* srcfile, int line){
		if(condition){
			throw new E(message, srcfile, line);
		}
	}
};

} /* namespace codablecash */

#endif /* BC_EXCEPTIONTHROWER_H_ */
