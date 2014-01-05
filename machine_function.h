/*
 * function.h
 *
 *  Created on: 20.01.2011
 *      Author: micha
 */
#pragma once
#ifndef MACHINE_FUNCTION_H_
#define MACHINE_FUNCTION_H_

#include <gmp.h>
#include <integer-fhe.h>

int Function_decode(fhe_int_t cipher,fhe_sk_t sk);
int Function_decode2(fhe_int_t *cipher,int size,fhe_sk_t sk);
void Function_encode(int size,int value,fhe_int_t *r,fhe_pk_t pk);

void Function_and2(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk);
void Function_and3(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_pk_t pk);
void Function_and4(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_pk_t pk);
void Function_and8(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,
								 fhe_int_t e,fhe_int_t f,fhe_int_t g,fhe_int_t h,fhe_pk_t pk);


void Function_xor(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk);

void Function_not(fhe_int_t res,fhe_int_t a,fhe_pk_t pk);

void Function_or2(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk);
void Function_or3(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_pk_t pk);
void Function_or4(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_pk_t pk);
void Function_or6(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_int_t e,fhe_int_t f,fhe_pk_t pk);
void Function_or15(fhe_int_t res,fhe_int_t a1,fhe_int_t a2,fhe_int_t a3,fhe_int_t a4,fhe_int_t a5,
								  fhe_int_t a6,fhe_int_t a7,fhe_int_t a8,fhe_int_t a9,fhe_int_t a10,
								  fhe_int_t a11,fhe_int_t a12,fhe_int_t a13,fhe_int_t a14,fhe_int_t a15,fhe_pk_t pk);

void Function_ha(fhe_int_t a,fhe_int_t b,fhe_int_t *res,fhe_pk_t pk);
void Function_fa(fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t *res,fhe_pk_t pk);


#endif /* FUNCTION_H_ */
