/*
 *  keygen.h
 *  integer-fhe
 *
 *  Created by Henning Perl on 01.03.10.
 *
 */

#pragma once
#ifndef INTEGER_FHE_H
#define INTEGER_FHE_H

//#include <gmp.h>
#include <assert.h>
//#include <fmpz_poly.h>
//#include <F_mpz_mod_poly.h>
#include "types.h"
#include "parameters.h"
//#include "util.h"

/** main function **/

void fhe_keygen(int *pk, int *sk);

void fhe_encrypt(int *c, int pk, int m);

int  fhe_decrypt(int c, int sk);

void fhe_recrypt(int *c, int pk);

void fhe_add(int *res, int a, int b, int pk);

void fhe_mul(int *res, int a, int b, int pk);

void fhe_fulladd(int *sum, int *c_out, int a, int b, int c_in, int pk);

void fhe_halfadd(int *sum, int *c_out, int a, int b, int pk);

#endif
