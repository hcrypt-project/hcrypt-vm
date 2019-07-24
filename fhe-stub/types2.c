/*
 * libbj.c
 *
 *  Created on: 05.10.2011
 *      Author: micha
 */



#include "types.h"
#include "types2.h"
#include "parameters.h"


fhe_sk_t *sk;
int errors;

void fhe_lib_init(fhe_sk_t *sk2)
{
	sk=sk2;
	errors=0;
}
/** I/O ***/
void fhe_pk_store(fhe_pk_t pk,char *s)
{
	FILE *f;
	f=fopen(s,"w");
	fprintf(f,"mlee public key (p,alpha,{c[i],B[i]}\n");

	fprintf(f,"%d\n",FHE_S1);
	fprintf(f,"%d\n",FHE_S2);
	fprintf(f,"%d\n",FHE_N);
	fprintf(f,"%d\n",FHE_T);
	fprintf(f,"%d\n",FHE_S);
	gmp_fprintf(f,"%Zd\n", pk->p);
	gmp_fprintf(f,"%Zd\n", pk->alpha);
	for (int i = 0; i < FHE_S1; i++) {
		gmp_fprintf(f,"%Zd\n%Zd\n", pk->c[i], pk->B[i]);
	}
	fclose(f);
}
int fhe_pk_loadkey(fhe_pk_t pk,char *filename)
{
	FILE *f;
	//static fhe_pk_t pk;
	char line[16384];
	//int i=pk->S1;

	//fhe_pk_init(pk);
	f=fopen(filename,"r");
	if(f==NULL)
		return 0;

	fgets(line,16384,f); //header line
	fgets(line,16384,f); //S1
	//pk->S1=atoi(line);
	fgets(line,16384,f); //S2
	//pk->S2=atoi(line);
	fgets(line,16384,f); //N
	//pk->N=atoi(line);
	fgets(line,16384,f); //T
	//pk->T=atoi(line);
	fgets(line,16384,f); //S
	//pk->S=atoi(line);
	fgets(line,16384,f); //p
	mpz_set_str(pk->p,line,10);
	fgets(line,16384,f); //alpha
	mpz_set_str(pk->alpha,line,10);

	for(int i=0;i<FHE_S1;i++)
	{
		fgets(line,16384,f);
		mpz_set_str(pk->c[i],line,10);
		fgets(line,16384,f);
		mpz_set_str(pk->B[i],line,10);
	}
	fclose(f);

	return 1;
}
void fhe_sk_store(fhe_sk_t sk,char *filename)
{
	FILE *f;

	f=fopen(filename,"w");
	fprintf(f,"mlee secret key (p,B)\n");
	fprintf(f,"%d\n",FHE_N);
	fprintf(f,"%d\n",FHE_MU);
	fprintf(f,"%d\n",FHE_LOG_NU);
	fprintf(f,"%d\n",FHE_S1);
	fprintf(f,"%d\n",FHE_S2);
	fprintf(f,"%d\n",FHE_T);
	fprintf(f,"%d\n",FHE_S);

	gmp_fprintf(f,"%Zd\n", sk->p);
	gmp_fprintf(f,"%Zd\n", sk->B);
	fclose(f);
}
int fhe_sk_loadkey(fhe_sk_t sk,char *filename)
{
	FILE *f;
	//static fhe_sk_t sk;
	char line[16384];

	//fhe_sk_init(sk);
	f=fopen(filename,"r");
	if(f==NULL)
		return 0;
	fgets(line,16384,f); //header line
	fgets(line,16384,f); //S1
	//sk->N=atoi(line);
	fgets(line,16384,f); //S2
	//sk->MU=atoi(line);
	fgets(line,16384,f); //N
	//sk->LOG_NU=atoi(line);
	fgets(line,16384,f); //T
	//sk->S1=atoi(line);
	fgets(line,16384,f); //S
	//sk->S2=atoi(line);
	fgets(line,16384,f); //S
	//sk->T=atoi(line);
	fgets(line,16384,f); //S
	//sk->S=atoi(line);

	fgets(line,16384,f); //p
	mpz_set_str(sk->p,line,10);
	fgets(line,16384,f); //B
	mpz_set_str(sk->B,line,10);
	fclose(f);

	return 1;
}

void fhe_int_init(fhe_int_t i)
{
	mpz_init(i->v);
	i->n=0;
}

void fhe_int_clear(fhe_int_t i)
{
	mpz_clear(i->v);
}

void fhe_int_set(fhe_int_t to,fhe_int_t from)
{
	mpz_set(to->v,from->v);
	to->n=from->n;
}

void
fhe_int_add(fhe_int_t res, fhe_int_t a, fhe_int_t b, fhe_pk_t pk)
{
	mpz_add(res->v, a->v, b->v);
	mpz_mod(res->v, res->v, pk->p);
	res->n=a->n+b->n+1;

	if(((fhe_decrypt(a->v,*sk)+fhe_decrypt(b->v,*sk))&1)!=fhe_decrypt(res->v,*sk))
	{
		printf("*** fhe_add error ***");
		errors++;
	}

	fhe_autorecrypt(res,pk);
}

void
fhe_int_mul(fhe_int_t res, fhe_int_t a, fhe_int_t b, fhe_pk_t pk)
{
	mpz_mul(res->v, a->v, b->v);
	mpz_mod(res->v, res->v, pk->p);
	res->n=a->n+b->n+1000;

	if(((fhe_decrypt(a->v,*sk)*fhe_decrypt(b->v,*sk))&1)!=fhe_decrypt(res->v,*sk))
	{
		printf("*** fhe_mul error ***");
		errors++;
	}

	fhe_autorecrypt(res,pk);
}

void fhe_autorecrypt(fhe_int_t a,fhe_pk_t pk)
{
	if(a->n>1100) //45000
	{
		fhe_recrypt(a->v,pk);
		a->n=0;
	}
}
