/*
 * bn_sqrt.h
 *
 *  Created on: 2023/01/31
 *      Author: iizuka
 */

#ifndef MOD_SQRT_BN_SQRT_H_
#define MOD_SQRT_BN_SQRT_H_

#include "gmp.h"

#define BN_R_NOT_A_SQUARE 10
#define BN_R_P_IS_NOT_PRIME 11
#define BN_R_TOO_MANY_ITERATIONS 100


int BN_mod_sqrt(mpz_t in, const mpz_t a, const mpz_t p);


#endif /* MOD_SQRT_BN_SQRT_H_ */
