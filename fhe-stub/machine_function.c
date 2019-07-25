/*
 * function.c
 *
 *  Created on: 20.01.2011
 *      Author: micha
 */

/*
 * Function.c
 *
 *  Created on: 30.07.2010
 *      Author: root
 */

/*
 * This class implements sample encrypted basic boolean functions
 *
 * and
 * or
 * xor
 * not
 * ha           half-adder
 * fa           full-adder
 *
 */

#include <stdlib.h>
#include "machine_function.h"
#include "integer-fhe.h"

int Function_opcountand;
int Function_opcountxor;
int Function_opcountnot;

int Function_decode(int cipher,int sk)
{
	return fhe_decrypt(cipher->v,sk);
}

int Function_decode2(int *cipher,int size,int sk)
{
	int i,m;
	int res=0;

	for(i=size-1;i>=0;i--)
	{
		m=fhe_decrypt(cipher[i]->v,sk);
		//printf("%d ",m);
		res|=m;
		if(i>0)
			res<<=1;
	}
	//printf("\n");
	return res;
}

void Function_encode(int size,int value,int *r,int pk)
{
	int i,m;
	int mask=1;

	for(i=0;i<size;i++,mask<<=1)
	{
		m=((value&mask)>0);
		//printf("%d-",m);
		fhe_encrypt(r[i]->v,pk,m);
		r[i]->n=0;
		//printf("%d ",fhe_decrypt(r[i],sk));
	}
	//printf("\n");
}

void Function_and2(int res,int a,int b,int pk)
{
	Function_opcountand++;
	fhe_mul(res,a,b,pk);
}

void Function_and3(int res,int a,int b,int c,int pk)
{
	int t;
	fhe_int_init(t);
	Function_and2(t,a,b,pk);
	Function_and2(res,t,c,pk);
	fhe_int_clear(t);
}

void Function_and4(int res,int a,int b,int c,int d,int pk)
{
	int t;
	fhe_int_init(t);
	Function_and3(t,a,b,c,pk);
	Function_and2(res,t,d,pk);
	fhe_int_clear(t);
}

void Function_and8(int res,int a,int b,int c,int d,
								 int e,int f,int g,int h,int pk)
{
	int t1,t2;
	fhe_int_init(t1);
	fhe_int_init(t2);

	Function_and4(t1,a,b,c,d,pk);
	Function_and4(t2,e,f,g,h,pk);
	Function_and2(res,t1,t2,pk);

	fhe_int_clear(t1);
	fhe_int_clear(t2);
}

void Function_or15(int res,int a1,int a2,int a3,int a4,int a5,
								  int a6,int a7,int a8,int a9,int a10,
								  int a11,int a12,int a13,int a14,int a15,int pk)
{
	int t1,t2,t3,t4;
	fhe_int_init(t1);
	fhe_int_init(t2);
	fhe_int_init(t3);
	fhe_int_init(t4);

	Function_or4(t1,a1,a2,a3,a4,pk);
	Function_or4(t2,a5,a6,a7,a8,pk);
	Function_or4(t3,a9,a10,a11,a12,pk);
	Function_or3(t4,a13,a14,a15,pk);
	Function_or4(res,t1,t2,t3,t4,pk);

	fhe_int_clear(t1);
	fhe_int_clear(t2);
	fhe_int_clear(t3);
	fhe_int_clear(t4);
}


void Function_xor(int res,int a,int b,int pk)
{
	Function_opcountxor++;
	fhe_add(res,a,b,pk);
}

void Function_not(int res,int a,int pk)
{
	int t;
	fhe_int_init(t);
	Function_opcountnot++;
	fhe_encrypt(t->v,pk,1);
	t->n=0;
	fhe_add(res,a,t,pk);
	fhe_int_clear(t);
}

void Function_or4(int res,int a,int b,int c,int d,int pk)
{
	int t;
	fhe_int_init(t);
	Function_or3(t,a,b,c,pk);
	Function_or2(res,t,d,pk);
	fhe_int_clear(t);
}

void Function_or3(int res,int a,int b,int c,int pk)
{
	int t;
	fhe_int_init(t);
	Function_or2(t,a,b,pk);
	Function_or2(res,t,c,pk);
	fhe_int_clear(t);
}

void Function_or2(int res,int a,int b,int pk)
{
	int ta,tx;
	fhe_int_init(ta);
	fhe_int_init(tx);
	Function_and2(ta,a,b,pk);
	Function_xor(tx,a,b,pk);
	Function_xor(res,tx,ta,pk);
	fhe_int_clear(ta);
	fhe_int_clear(tx);
}

void Function_or6(int res,int a,int b,int c,int d,int e,int f,int pk)
{
	int t;
	fhe_int_init(t);
	Function_or3(t,a,b,c,pk);
	Function_or4(res,t,d,e,f,pk);
	fhe_int_clear(t);
}

void Function_ha(int a,int b,int *res,int pk)
{
	Function_xor(res[0],a,b,pk);
	Function_and2(res[1],a,b,pk);
}

void Function_fa(int a,int b,int c,int *res,int pk)
{
	int ha1[2],ha2[2];
	fhe_int_init(ha1[0]);
	fhe_int_init(ha1[1]);
	fhe_int_init(ha2[0]);
	fhe_int_init(ha2[1]);

	Function_ha(a,b,ha1,pk);
	Function_ha(ha1[0],c,ha2,pk);
	fhe_int_set(res[0],ha2[0]);
	Function_or2(res[1],ha1[1],ha2[1],pk);

	fhe_int_clear(ha1[0]);
	fhe_int_clear(ha1[1]);
	fhe_int_clear(ha2[0]);
	fhe_int_clear(ha2[1]);
}
