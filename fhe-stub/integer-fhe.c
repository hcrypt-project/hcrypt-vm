/*
 *  keygen.h
 *  integer-fhe
 *
 *  Created by Henning Perl on 01.03.10.
 *
 */

#pragma once

#include "integer-fhe.h"

/** main function **/

void fhe_keygen(int *pk, int *sk)
{
    *pk=0;
    *sk=0;
}

void fhe_encrypt(int *c, int pk, int m)
{
    *c=m&1;
}

int  fhe_decrypt(int c, int sk)
{
    return c&1;
}

void fhe_recrypt(int *c, int pk)
{
    *c=c&1;
}

void fhe_add(int *res, int a, int b, int pk)
{
    *res=(a+b)&1;
}

void fhe_mul(int *res, int a, int b, int pk)
{
    *res=(a*b)&1;
}

void fhe_fulladd(int *sum, int *c_out, int a, int b, int c_in, int pk)
{
    int temp;
    
    temp=(a+b)&1;
    *sum=(temp+c_in)&1;
    
    temp=(a*b)&1;
    temp=temp+(c_in*a)&1;
    temp=temp+(c_in*b)&1;
    
    *c_out=temp&1;
    
    /*
    mpz_t temp;
    mpz_init(temp);
    
    mpz_add(temp, a, b);
    mpz_add(temp, temp, c_in);
    mpz_mod(sum, temp, pk->p);
    
    mpz_mul(temp, a, b);
    mpz_addmul(temp, c_in, a);
    mpz_addmul(temp, c_in, b);
    mpz_mod(c_out, temp, pk->p);
    
    mpz_clear(temp);
    */
}

void fhe_halfadd(int *sum, int *c_out, int a, int b, int pk)
{
    *sum=(a+b)&1;
    *c_out=(a*b)&1;
    
    /*
    mpz_add(sum, a, b);
    mpz_mod(sum, sum, pk->p);
    
    mpz_mul(c_out, a, b);
    mpz_mod(c_out, c_out, pk->p);
     */
}

#endif
