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
#include "integer-fhe.h"
#include "types2.h"

int Function_decode(int cipher,int sk);
int Function_decode2(int *cipher,int size,int sk);
void Function_encode(int size,int value,int *r,int pk);

void Function_and2(int res,int a,int b,int pk);
void Function_and3(int res,int a,int b,int c,int pk);
void Function_and4(int res,int a,int b,int c,int d,int pk);
void Function_and8(int res,int a,int b,int c,int d,
								 int e,int f,int g,int h,int pk);


void Function_xor(int res,int a,int b,int pk);

void Function_not(int res,int a,int pk);

void Function_or2(int res,int a,int b,int pk);
void Function_or3(int res,int a,int b,int c,int pk);
void Function_or4(int res,int a,int b,int c,int d,int pk);
void Function_or6(int res,int a,int b,int c,int d,int e,int f,int pk);
void Function_or15(int res,int a1,int a2,int a3,int a4,int a5,
								  int a6,int a7,int a8,int a9,int a10,
								  int a11,int a12,int a13,int a14,int a15,int pk);

void Function_ha(int a,int b,int *res,int pk);
void Function_fa(int a,int b,int c,int *res,int pk);


#endif /* FUNCTION_H_ */
