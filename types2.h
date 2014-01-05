/*
 * bjtypes.h
 *
 *  Created on: 05.10.2011
 *      Author: micha
 */

#ifndef BJTYPES_H_
#define BJTYPES_H_


#include <stdio.h>
#include <gmp.h>
#include "integer-fhe.h"

typedef struct {
	mpz_t v;
	int n;
} _fhe_int;
typedef _fhe_int fhe_int_t[1];

void fhe_lib_init(fhe_sk_t *sk2);

void fhe_int_init(fhe_int_t i);

void fhe_int_clear(fhe_int_t i);

void fhe_int_set(fhe_int_t to,fhe_int_t from);



/** I/O **/
void fhe_pk_store(fhe_pk_t pk,char *filename);

void fhe_sk_store(fhe_sk_t sk,char *filename);

int fhe_pk_loadkey(fhe_pk_t pk,char *filename);

int fhe_sk_loadkey(fhe_sk_t sk,char *filename);

void fhe_int_add(fhe_int_t res, fhe_int_t a, fhe_int_t b, fhe_pk_t pk);

void fhe_int_mul(fhe_int_t res, fhe_int_t a, fhe_int_t b, fhe_pk_t pk);

void fhe_autorecrypt(fhe_int_t a,fhe_pk_t pk);


#endif /* BJTYPES_H_ */
