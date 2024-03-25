/*
 * Copyright 2000-2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */
/*
 * bn_sqrt.cpp
 *
 *  Created on: 2023/01/31
 *      Author: iizuka
 */
/*
 * Copyright 2000-2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */
#include "bn_sqrt.h"
#include "bn_mpz.h"

// #include "internal/cryptlib.h"
// #include "bn_local.h"

int BN_mod_sqrt(mpz_t ret, const mpz_t a, const mpz_t p)
/*
 * Returns 'ret' such that ret^2 == a (mod p), using the Tonelli/Shanks
 * algorithm (cf. Henri Cohen, "A Course in Algebraic Computational Number
 * Theory", algorithm 1.5.1). 'p' must be prime, otherwise an error or
 * an incorrect "result" will be returned.
 */
{
	int err = 1;
	int r;
	mpz_t A, b, q, t, x, y;
	int e, i, j;

	mpz_init(A); // A = BN_CTX_get(ctx);
	mpz_init(b); // b = BN_CTX_get(ctx);
	mpz_init(q); // q = BN_CTX_get(ctx);
	mpz_init(t); // t = BN_CTX_get(ctx);
	mpz_init(x); // x = BN_CTX_get(ctx);
	mpz_init(y); // y = BN_CTX_get(ctx);
	if (y == NULL)
		goto end;

	/* A = a mod p */
	BN_nnmod(A, a, p);

	/* now write  |p| - 1  as  2^e*q  where  q  is odd */
	e = 1;
	while (!BN_is_bit_set(p, e)){
		e++;
	}
	/* we'll set  q  later (if needed) */

	if (e == 1) {
		/*-
		 * The easy case:  (|p|-1)/2  is odd, so 2 has an inverse
		 * modulo  (|p|-1)/2,  and square roots can be computed
		 * directly by modular exponentiation.
		 * We have
		 *     2 * (|p|+1)/4 == 1   (mod (|p|-1)/2),
		 * so we can use exponent  (|p|+1)/4,  i.e.  (|p|-3)/4 + 1.
		 */
		BN_rshift(q, p, 2);
		BN_potsitive(q); //q->neg = 0;
		BN_add_word(q, 1);
		BN_mod_exp(ret, A, q, p);

		err = 0;
		goto vrfy;
	}

	if (e == 2) {
		/*-
		 * |p| == 5  (mod 8)
		 *
		 * In this case  2  is always a non-square since
		 * Legendre(2,p) = (-1)^((p^2-1)/8)  for any odd prime.
		 * So if  a  really is a square, then  2*a  is a non-square.
		 * Thus for
		 *      b := (2*a)^((|p|-5)/8),
		 *      i := (2*a)*b^2
		 * we have
		 *     i^2 = (2*a)^((1 + (|p|-5)/4)*2)
		 *         = (2*a)^((p-1)/2)
		 *         = -1;
		 * so if we set
		 *      x := a*b*(i-1),
		 * then
		 *     x^2 = a^2 * b^2 * (i^2 - 2*i + 1)
		 *         = a^2 * b^2 * (-2*i)
		 *         = a*(-i)*(2*a*b^2)
		 *         = a*(-i)*i
		 *         = a.
		 *
		 * (This is due to A.O.L. Atkin,
		 * Subject: Square Roots and Cognate Matters modulo p=8n+5.
		 * URL: https://listserv.nodak.edu/cgi-bin/wa.exe?A2=ind9211&L=NMBRTHRY&P=4026
		 * November 1992.)
		 */

		/* t := 2*a */
		BN_mod_lshift1_quick(t, A, p);

		/* b := (2*a)^((|p|-5)/8) */
		BN_rshift(q, p, 3);
		BN_potsitive(q); // q->neg = 0;
		BN_mod_exp(b, t, q, p);

		/* y := b^2 */
		BN_mod_sqr(y, b, p);

		/* t := (2*a)*b^2 - 1 */
		BN_mod_mul(t, t, y, p);

		BN_sub_word(t, 1);

		/* x = a*b*t */
		BN_mod_mul(x, A, b, p);
		BN_mod_mul(x, x, t, p);

		BN_copy(ret, x);
		err = 0;
		goto vrfy;
	}

    /*
     * e > 2, so we really have to use the Tonelli/Shanks algorithm. First,
     * find some y that is not a square.
     */
    BN_copy(q, p);           /* use 'q' as temp */
    BN_potsitive(q); //q->neg = 0;
    i = 2;
    do {
        /*
         * For efficiency, try small numbers first; if this fails, try random
         * numbers.
         */
		if (i < 22) {
			BN_set_word(y, i);
		} else {
			BN_priv_rand_ex(y, BN_num_bits(p));

			if (BN_ucmp(y, p) >= 0) {
				//if (!(p->neg ? BN_add : BN_sub) (y, y, p))
				//    goto end;
				if (BN_is_negative(p)) {
					BN_add(y, y, p);
				} else {
					BN_sub(y, y, p);
				}
			}
			/* now 0 <= y < |p| */
			if (BN_is_zero(y))
				BN_set_word(y, i);
		}

		r = BN_kronecker(y, q); /* here 'q' is |p| */
		if (r < -1)
			goto end;
		if (r == 0) {
			/* m divides p */
			//ERR_raise(ERR_LIB_BN, BN_R_P_IS_NOT_PRIME);
			err = BN_R_P_IS_NOT_PRIME;
			goto end;
		}
    }
    while (r == 1 && ++i < 82);

	if (r != -1) {
		/*
		 * Many rounds and still no non-square -- this is more likely a bug
		 * than just bad luck. Even if p is not prime, we should have found
		 * some y such that r == -1.
		 */
		//ERR_raise(ERR_LIB_BN, BN_R_TOO_MANY_ITERATIONS);
		err = BN_R_TOO_MANY_ITERATIONS;
		goto end;
	}

    /* Here's our actual 'q': */
    BN_rshift(q, q, e);

	/*
	 * Now that we have some non-square, we can find an element of order 2^e
	 * by computing its q'th power.
	 */
	BN_mod_exp(y, y, q, p);
	if (BN_is_one(y)) {
		//ERR_raise(ERR_LIB_BN, BN_R_P_IS_NOT_PRIME);
		err = BN_R_P_IS_NOT_PRIME;
		goto end;
	}

    /*-
     * Now we know that (if  p  is indeed prime) there is an integer
     * k,  0 <= k < 2^e,  such that
     *
     *      a^q * y^k == 1   (mod p).
     *
     * As  a^q  is a square and  y  is not,  k  must be even.
     * q+1  is even, too, so there is an element
     *
     *     X := a^((q+1)/2) * y^(k/2),
     *
     * and it satisfies
     *
     *     X^2 = a^q * a     * y^k
     *         = a,
     *
     * so it is the square root that we are looking for.
     */

    /* t := (q-1)/2  (note that  q  is odd) */
    BN_rshift1(t, q);

    /* x := a^((q-1)/2) */
    if (BN_is_zero(t)) {        /* special case: p = 2^e + 1 */
    	BN_nnmod(t, A, p);
       if (BN_is_zero(t)) {
            /* special case: a == 0  (mod p) */
            BN_zero(ret);
            err = 0;
            goto end;
       } else if (!BN_one(x)) {
            goto end;
        }
    }else {
    	BN_mod_exp(x, A, t, p);
       if (BN_is_zero(x)) {
            /* special case: a == 0  (mod p) */
            BN_zero(ret);
            err = 0;
            goto end;
        }
    }

    /* b := a*x^2  (= a^q) */
    BN_mod_sqr(b, x, p);

    BN_mod_mul(b, b, A, p);

    /* x := a*x    (= a^((q+1)/2)) */
    BN_mod_mul(x, x, A, p);

	while (1) {
		/*-
		 * Now  b  is  a^q * y^k  for some even  k  (0 <= k < 2^E
		 * where  E  refers to the original value of  e,  which we
		 * don't keep in a variable),  and  x  is  a^((q+1)/2) * y^(k/2).
		 *
		 * We have  a*b = x^2,
		 *    y^2^(e-1) = -1,
		 *    b^2^(e-1) = 1.
		 */

		if (BN_is_one(b)) {
			BN_copy(ret, x);
			err = 0;
			goto vrfy;
		}

		/* Find the smallest i, 0 < i < e, such that b^(2^i) = 1. */
		for (i = 1; i < e; i++) {
			if (i == 1) {
				BN_mod_sqr(t, b, p);

			} else {
				BN_mod_mul(t, t, t, p);
			}
			if (BN_is_one(t))
				break;
		}
		/* If not found, a is not a square or p is not prime. */
		if (i >= e) {
			//ERR_raise(ERR_LIB_BN, BN_R_NOT_A_SQUARE);
			err = BN_R_NOT_A_SQUARE;
			goto end;
		}

		/* t := y^2^(e - i - 1) */
		BN_copy(t, y);
		for (j = e - i - 1; j > 0; j--) {
			BN_mod_sqr(t, t, p);
		}
		BN_mod_mul(y, t, t, p);
		BN_mod_mul(x, x, t, p);
		BN_mod_mul(b, b, y, p);
		e = i;
	}

 vrfy:
	if (!err) {
		/*
		 * verify the result -- the input might have been not a square (test
		 * added in 0.9.8)
		 */

		BN_mod_sqr(x, ret, p);

		if (!err && 0 != BN_cmp(x, A)) {
			//ERR_raise(ERR_LIB_BN, BN_R_NOT_A_SQUARE);
			//err = 1;
			err = BN_R_NOT_A_SQUARE;
		}
	}

	end:

    mpz_clear(A); // A = BN_CTX_get(ctx);
    mpz_clear(b); // b = BN_CTX_get(ctx);
    mpz_clear(q); // q = BN_CTX_get(ctx);
    mpz_clear(t); // t = BN_CTX_get(ctx);
    mpz_clear(x); // x = BN_CTX_get(ctx);
    mpz_clear(y); // y = BN_CTX_get(ctx);

	return err;
}

