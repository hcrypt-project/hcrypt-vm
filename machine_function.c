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
#include <machine_function.h>
#include <integer-fhe.h>

int Function_opcountand;
int Function_opcountxor;
int Function_opcountnot;

int Function_decode(fhe_int_t cipher,fhe_sk_t sk)
{
	return fhe_decrypt(cipher->v,sk);
}

int Function_decode2(fhe_int_t *cipher,int size,fhe_sk_t sk)
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

void Function_encode(int size,int value,fhe_int_t *r,fhe_pk_t pk)
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

void Function_and2(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk)
{
	Function_opcountand++;
	fhe_mul(res,a,b,pk);
}

void Function_and3(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_and2(t,a,b,pk);
	Function_and2(res,t,c,pk);
	fhe_int_clear(t);
}

void Function_and4(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_and3(t,a,b,c,pk);
	Function_and2(res,t,d,pk);
	fhe_int_clear(t);
}

void Function_and8(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,
								 fhe_int_t e,fhe_int_t f,fhe_int_t g,fhe_int_t h,fhe_pk_t pk)
{
	fhe_int_t t1,t2;
	fhe_int_init(t1);
	fhe_int_init(t2);

	Function_and4(t1,a,b,c,d,pk);
	Function_and4(t2,e,f,g,h,pk);
	Function_and2(res,t1,t2,pk);

	fhe_int_clear(t1);
	fhe_int_clear(t2);
}

void Function_or15(fhe_int_t res,fhe_int_t a1,fhe_int_t a2,fhe_int_t a3,fhe_int_t a4,fhe_int_t a5,
								  fhe_int_t a6,fhe_int_t a7,fhe_int_t a8,fhe_int_t a9,fhe_int_t a10,
								  fhe_int_t a11,fhe_int_t a12,fhe_int_t a13,fhe_int_t a14,fhe_int_t a15,fhe_pk_t pk)
{
	fhe_int_t t1,t2,t3,t4;
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


void Function_xor(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk)
{
	Function_opcountxor++;
	fhe_add(res,a,b,pk);
}

void Function_not(fhe_int_t res,fhe_int_t a,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_opcountnot++;
	fhe_encrypt(t->v,pk,1);
	t->n=0;
	fhe_add(res,a,t,pk);
	fhe_int_clear(t);
}

void Function_or4(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_or3(t,a,b,c,pk);
	Function_or2(res,t,d,pk);
	fhe_int_clear(t);
}

void Function_or3(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_or2(t,a,b,pk);
	Function_or2(res,t,c,pk);
	fhe_int_clear(t);
}

void Function_or2(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_pk_t pk)
{
	fhe_int_t ta,tx;
	fhe_int_init(ta);
	fhe_int_init(tx);
	Function_and2(ta,a,b,pk);
	Function_xor(tx,a,b,pk);
	Function_xor(res,tx,ta,pk);
	fhe_int_clear(ta);
	fhe_int_clear(tx);
}

void Function_or6(fhe_int_t res,fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t d,fhe_int_t e,fhe_int_t f,fhe_pk_t pk)
{
	fhe_int_t t;
	fhe_int_init(t);
	Function_or3(t,a,b,c,pk);
	Function_or4(res,t,d,e,f,pk);
	fhe_int_clear(t);
}

void Function_ha(fhe_int_t a,fhe_int_t b,fhe_int_t *res,fhe_pk_t pk)
{
	Function_xor(res[0],a,b,pk);
	Function_and2(res[1],a,b,pk);
}

void Function_fa(fhe_int_t a,fhe_int_t b,fhe_int_t c,fhe_int_t *res,fhe_pk_t pk)
{
	fhe_int_t ha1[2],ha2[2];
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
