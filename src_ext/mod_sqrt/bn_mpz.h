/*
 * bn_mpz.h
 *
 *  Created on: 2023/01/31
 *      Author: iizuka
 */

#ifndef MOD_SQRT_BN_MPZ_H_
#define MOD_SQRT_BN_MPZ_H_

#include <gmp.h>

#include "cstdint"

#include "ctime"

int BN_kronecker(const mpz_t a, const mpz_t b);

/* A = a mod p */
inline void BN_nnmod(mpz_t A, const mpz_t a, const mpz_t p){
	mpz_mod(A, a, p);
}

inline bool BN_is_bit_set(const mpz_t p, int e){
	mp_bitcnt_t bit_index = e;

	int bit = mpz_tstbit(p, bit_index);
	return bit > 0;
}

inline bool BN_is_odd(const mpz_t a){
	return BN_is_bit_set(a, 0);
}

inline void BN_set_word(mpz_t a, uint64_t w) {
	mpz_set_ui(a, w);
}

inline uint64_t BN_num_bits(const mpz_t a){
	int sib = mpz_sizeinbase(a, 2);
	return sib;
}

inline void BN_rshift(mpz_t op, const mpz_t p, int num){
	mpz_fdiv_q_2exp(op, p, num);
}

inline void BN_rshift1(mpz_t t, const mpz_t q){
	mpz_fdiv_q_2exp(t, q, 1);
}

/* t := 2*a  */
inline void BN_mod_lshift1_quick(mpz_t t, mpz_t A, const mpz_t p){
	long int n = mpz_get_si(p);
	mpz_mul_2exp(t, A, n);
}

// BN_add_word() adds w to a (a+=w).
inline void BN_add_word(mpz_t a, uint64_t w){
	mpz_add_ui(a, a, w);
}

// subtracts w from a (a-=w).
inline void BN_sub_word(mpz_t a, uint64_t w){
	mpz_sub_ui(a, a, w);
}

inline void BN_add(mpz_t r, const mpz_t a, const mpz_t b){
	mpz_add(r, a, b);
}

inline void BN_sub(mpz_t r, const mpz_t a, const mpz_t b){
	mpz_sub(r, a, b);
}

// r=a^p % m
inline void BN_mod_exp(mpz_t r, mpz_t a, mpz_t p, const mpz_t m){
	//mpz_powm (mpz_t rop, mpz_t base, mpz_t exp, mpz_t mod)
	mpz_powm(r, a, p, m);
}
// r := a^2 mod m
inline void BN_mod_sqr(mpz_t r, mpz_t a, const mpz_t m){
	mpz_powm_ui(r, a, 2, m);
}

inline void BN_mod_mul(mpz_t r, mpz_t a, mpz_t b, const mpz_t m){
	mpz_mul(r, a, b);
	mpz_mod(r, r, m);
}

inline void BN_copy(mpz_t to, const mpz_t from){
	mpz_set(to, from);
}

inline bool BN_is_zero(const mpz_t a){
	mpz_t zero;
	mpz_init_set_ui(zero, (uint64_t)0);

	int diff = mpz_cmp(a, zero);
	mpz_clear(zero);

	return (diff == 0);
}

inline bool BN_is_one(const mpz_t a){
	mpz_t one;
	mpz_init_set_ui(one, (uint64_t)1);

	int diff = mpz_cmp(a, one);
	mpz_clear(one);

	return (diff == 0);
}

inline int BN_cmp(const mpz_t a, const mpz_t b){
	int diff = mpz_cmp(a, b);
	return diff;
}

inline int BN_ucmp(const mpz_t a, const mpz_t b){
	mpz_t absa, absb;
	mpz_init(absa);
	mpz_init(absb);

	mpz_abs(absa, a);
	mpz_abs(absb, b);

	int diff = mpz_cmp(absa, absb);

	mpz_clear(absa);
	mpz_clear(absb);

	return diff;
}

inline void BN_zero(mpz_t op){
	mpz_set_ui(op, (uint64_t)0);
}

inline int BN_one(mpz_t op){
	mpz_set_ui(op, (uint64_t)1);
	return 1;
}

inline bool BN_is_negative(const mpz_t op){
	int ret = mpz_sgn(op);
	return ret < 0;
}

inline void BN_potsitive(mpz_t op){
	if(BN_is_negative(op)){
		mpz_mul_si(op, op, -1);
	}
}

inline void BN_priv_rand_ex(mpz_t rnd, int bits){
	gmp_randstate_t state;
	gmp_randinit_default(state);

	// use microsec
	struct timespec startTime;
	clock_gettime(CLOCK_REALTIME, &startTime);
	uint64_t microsec = startTime.tv_sec * 1000000 + (startTime.tv_nsec / 1000);

	// set seed
	gmp_randseed_ui(state, microsec);

	mpz_urandomb(rnd, state, bits);

	gmp_randclear(state);
}

inline bool BN_abs_is_word(const mpz_t a, const uint64_t w){
	mpz_t absa;
	mpz_init(absa);

	mpz_abs(absa, a);

	int diff = mpz_cmp_ui(absa, w);

	mpz_clear(absa);

	return diff;
}

#endif /* MOD_SQRT_BN_MPZ_H_ */
