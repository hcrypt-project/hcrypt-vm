/*
 * CU.c
 *
 *  Created on: 30.07.2010
 *      Author: root
 */

/*
 * This class implements an encrypted control unit
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <gmp.h>

#include "integer-fhe.h"
#include "machine_function.h"
#include "machine_memory.h"
#include "types2.h"

long CU_process(fhe_pk_t pk,fhe_sk_t sk);
void copyreg(fhe_int_t *a,fhe_int_t *b);
void dump(char *cmd,fhe_int_t *reg,int length, fhe_int_t carry,fhe_sk_t sk);

//int debug=1;
static fhe_int_t VALUE_ONE;
static fhe_int_t VALUE_ZERO;
static fhe_int_t ALU_carry;
static fhe_int_t ALU_zero;
static fhe_int_t ALU_minus;
static fhe_int_t t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15;
static fhe_int_t carry_rol;
static fhe_int_t carry_ror;
static fhe_int_t carry_add;
static fhe_int_t carry_add1;
static fhe_int_t carry_add2;
static fhe_int_t carry_clc;
static fhe_int_t carry_sec;
static fhe_int_t pc[8]; //0
static fhe_int_t ac[13];
static fhe_int_t b1[13];
static fhe_int_t nb1[13];
static fhe_int_t b_rol[13];
static fhe_int_t b_ror[13];
static fhe_int_t b_add[13];
static fhe_int_t b_clc[13];
static fhe_int_t b_sec[13];
static fhe_int_t b_xor[13];
static fhe_int_t b_and[13];
static fhe_int_t b_or[13];
//static int b_beq[13];
static fhe_int_t b_cmp[13];
static fhe_int_t cmd_store;
static fhe_int_t ncmd_store;
static fhe_int_t cmd_rol;
static fhe_int_t cmd_load;
static fhe_int_t cmd_ror;
static fhe_int_t cmd_add;
static fhe_int_t cmd_clc;
static fhe_int_t cmd_sec;
static fhe_int_t cmd_xor;
static fhe_int_t cmd_and;
static fhe_int_t cmd_or;
static fhe_int_t cmd_beq;
static fhe_int_t cmd_jmp;
static fhe_int_t cmd_la;
static fhe_int_t cmd_bmi;
static fhe_int_t cmd_cmp;
static fhe_int_t ncmd_cmp;
static fhe_int_t cmd_a;
static fhe_int_t ncmd_a;
static fhe_int_t load_val[13];
static fhe_int_t zero2;
static fhe_int_t zero1;
static fhe_int_t b_cmp_0[8];
static fhe_int_t temp31[8];
static fhe_int_t temp32[8];
//static int b_add_1[8];
//static int b_add_2[8];
static fhe_int_t carry_non;

static fhe_int_t pc_linear[8];
static fhe_int_t *pc_jump;
static fhe_int_t *pc_branch;
static fhe_int_t load_arg[13];
static fhe_int_t one_field[8];//={1,2,2,2,2,2,2,2};

static char command_list[32][5]={"NOP","CMP","BMI","La","J","BEQ","OR","AND","XOR","SEC","CLC","ADD","ROR","ROL","L","STa","NOPa","CMPa","BMIa","La","Ja","BEQa","ORa","ANDa","XORa","SECa","CLCa","ADDa","RORa","ROLa","La","STa"};
static int cmd_index;
static int cmd_arg;
static int dumpid=0;


void CU_init(fhe_pk_t pk)
{
	int i;

	fhe_int_init(Memory_READ);
		fhe_encrypt(Memory_READ->v,pk,1);
	fhe_int_init(VALUE_ONE);
		fhe_encrypt(VALUE_ONE->v,pk,1);
	fhe_int_init(VALUE_ZERO);
		fhe_encrypt(VALUE_ZERO->v,pk,0);
	fhe_int_init(ALU_carry);
		fhe_encrypt(ALU_carry->v,pk,0);
	fhe_int_init(ALU_zero);
		fhe_encrypt(ALU_zero->v,pk,0);
	fhe_int_init(ALU_minus);
		fhe_encrypt(ALU_minus->v,pk,0);
	fhe_int_init(t1);
	fhe_int_init(t2);
	fhe_int_init(t3);
	fhe_int_init(t4);
	fhe_int_init(t5);
	fhe_int_init(t6);
	fhe_int_init(t7);
	fhe_int_init(t8);
	fhe_int_init(t9);
	fhe_int_init(t10);
	fhe_int_init(t12);
	fhe_int_init(t13);
	fhe_int_init(t11);
	fhe_int_init(t14);
	fhe_int_init(t15);
	fhe_int_init(zero2);
	fhe_int_init(zero1);
	fhe_int_init(carry_rol);
	fhe_int_init(carry_ror);
	fhe_int_init(carry_add);
	fhe_int_init(carry_add1);
	fhe_int_init(carry_add2);
	fhe_int_init(carry_clc);
	fhe_int_init(carry_sec);
	fhe_int_init(cmd_store);
	fhe_int_init(ncmd_store);
	fhe_int_init(cmd_rol);
	fhe_int_init(cmd_load);
	fhe_int_init(cmd_ror);
	fhe_int_init(cmd_add);
	fhe_int_init(cmd_clc);
	fhe_int_init(cmd_sec);
	fhe_int_init(cmd_xor);
	fhe_int_init(cmd_and);
	fhe_int_init(cmd_or);
	fhe_int_init(cmd_beq);
	fhe_int_init(cmd_jmp);
	fhe_int_init(cmd_la);
	fhe_int_init(cmd_bmi);
	fhe_int_init(cmd_cmp);
	fhe_int_init(ncmd_cmp);
	fhe_int_init(cmd_a);
	fhe_int_init(ncmd_a);
	fhe_int_init(carry_non);

	for(i=0;i<13;i++)
	{
		//static int b_beq[13];
		if(i<8)
		{
			fhe_int_init(b_cmp_0[i]);
			fhe_int_init(temp31[i]);
			fhe_int_init(temp32[i]);
			fhe_int_init(pc[i]); //0
			fhe_int_init(pc_linear[i]);
			fhe_int_init(one_field[i]);	//={1,2,2,2,2,2,2,2};
		}
		fhe_int_init(load_arg[i]);
		fhe_int_init(ac[i]);
		fhe_int_init(b1[i]);
		fhe_int_init(nb1[i]);
		fhe_int_init(b_rol[i]);
		fhe_int_init(b_ror[i]);
		fhe_int_init(b_add[i]);
		fhe_int_init(b_clc[i]);
		fhe_int_init(b_sec[i]);
		fhe_int_init(b_xor[i]);
		fhe_int_init(b_and[i]);
		fhe_int_init(b_or[i]);
		fhe_int_init(b_cmp[i]);
		fhe_int_init(load_val[i]);
	}

	for(i=1;i<8;i++)
		fhe_encrypt(one_field[i]->v,pk,0);
	fhe_encrypt(one_field[0]->v,pk,1);
}

long CU_process(fhe_pk_t pk,fhe_sk_t sk)
{
    static int i=0;
	//long t=System.currentTimeMillis();
	//while(true)

    if(debug) puts("___START_PROCESS___");
    if(debug) dump("PC",pc,8,NULL,sk);
    if(debug) dump("AC",ac,8,ALU_carry,sk);

    //ALU.minus=b1[7];
    if(debug){puts("PHASE 1: fetch");fflush(stdout);}
    //PHASE 1: fetch
    Memory_access2(pc,ac,Memory_READ,b1,pk,sk);
    if(debug) dump("load",b1,13,ALU_carry,sk);

    //Memory_access2(b1,ac,Memory_READ,load_arg,pk,sk);
    //if(debug) dump("aload",load_arg,8,ALU_carry,sk);

    if(debug){puts("PHASE 2: decode");fflush(stdout);}
    //PHASE 2: decode

    for(i=8;i<12;i++)
    	Function_not(nb1[i],b1[i],pk);

    if(debug)
    {
    	printf("cmd=");
    	for(i=8;i<13;i++)
    		printf("%d(%d) ",fhe_decrypt(b1[i]->v,sk),b1[i]->n);
		printf("\n");
    }

    //																		8->11
    Function_and4(cmd_store,b1[11],b1[10],b1[9],b1[8],pk);				//	1 1 1 1 store
    if(debug) printf("cmd_store=%d\n",fhe_decrypt(cmd_store->v,sk));
    Function_and4(cmd_load,b1[11],b1[10],b1[9],nb1[8],pk);				//	0 1 1 1 load
    if(debug) printf("cmd_load=%d\n",fhe_decrypt(cmd_load->v,sk));
    Function_and4(cmd_rol,b1[11],b1[10],nb1[9],b1[8],pk);				//	1 0 1 1 rol
    if(debug) printf("cmd_rol=%d\n",fhe_decrypt(cmd_rol->v,sk));
    Function_and4(cmd_ror,b1[11],b1[10],nb1[9],nb1[8],pk);				//	0 0 1 1 ror
    if(debug) printf("cmd_ror=%d\n",fhe_decrypt(cmd_ror->v,sk));
    Function_and4(cmd_add,b1[11],nb1[10],b1[9],b1[8],pk);				//	1 1 0 1 add
    if(debug) printf("cmd_add=%d\n",fhe_decrypt(cmd_add->v,sk));
    Function_and4(cmd_clc,b1[11],nb1[10],b1[9],nb1[8],pk);				//	0 1 0 1 clc
    if(debug) printf("cmd_clc=%d\n",fhe_decrypt(cmd_clc->v,sk));
    Function_and4(cmd_sec,b1[11],nb1[10],nb1[9],b1[8],pk);				//	1 0 0 1 sec
    if(debug) printf("cmd_sec=%d\n",fhe_decrypt(cmd_sec->v,sk));
    Function_and4(cmd_xor,b1[11],nb1[10],nb1[9],nb1[8],pk);				//	0 0 0 1 xor
    if(debug) printf("cmd_xor=%d\n",fhe_decrypt(cmd_xor->v,sk));
    Function_and4(cmd_and,nb1[11],b1[10],b1[9],b1[8],pk);				//	1 1 1 0 and
    if(debug) printf("cmd_and=%d\n",fhe_decrypt(cmd_and->v,sk));
    Function_and4(cmd_or,nb1[11],b1[10],b1[9],nb1[8],pk);				//	0 1 1 0 or
    if(debug) printf("cmd_or=%d\n",fhe_decrypt(cmd_or->v,sk));
    Function_and4(cmd_beq,nb1[11],b1[10],nb1[9],b1[8],pk);				//	1 0 1 0 beq
    if(debug) printf("cmd_beq=%d\n",fhe_decrypt(cmd_beq->v,sk));
    Function_and4(cmd_jmp,nb1[11],b1[10],nb1[9],nb1[8],pk);				//	0 0 1 0 jmp
    if(debug) printf("cmd_jmp=%d\n",fhe_decrypt(cmd_jmp->v,sk));
    Function_and4(cmd_la,nb1[11],nb1[10],b1[9],b1[8],pk);				//	1 1 0 0 la
    if(debug) printf("cmd_la=%d\n",fhe_decrypt(cmd_la->v,sk));
    Function_and4(cmd_bmi,nb1[11],nb1[10],b1[9],nb1[8],pk);				//	0 1 0 0 bmi
    if(debug) printf("cmd_bmi=%d\n",fhe_decrypt(cmd_bmi->v,sk));
    Function_and4(cmd_cmp,nb1[11],nb1[10],nb1[9],b1[8],pk);				//	1 0 0 0 cmp
    if(debug) printf("cmd_cmp=%d\n",fhe_decrypt(cmd_cmp->v,sk));
    fhe_int_set(cmd_a,b1[12]);
    if(debug) printf("cmd_a=%d\n",fhe_decrypt(cmd_a->v,sk));

	//printf("load=%d store=%d cmp=%d\n",cmd_load&1,cmd_store&1,cmd_cmp&1);
    if(debug){puts("PHASE 3: execute");fflush(stdout);}
    //PHASE 3: execute
	//XXX cmd_arg=(b1[0]&1)+((b1[1]&1)*2)+((b1[2]&1)*4)+((b1[3]&1)*8)+((b1[4]&1)*16)+((b1[5]&1)*32)+((b1[6]&1)*64)+((b1[7]&1)*128);
	//XXX cmd_index=((b1[12]&1)*16)+((b1[11]&1)*8)+((b1[10]&1)*4)+((b1[9]&1)*2)+(b1[8]&1);
	//XXX if(debug)
	//XXX	printf("CMD=%d (%s %d)\n",cmd_index,command_list[cmd_index],cmd_arg);
    //cmp
	Function_not(ncmd_a,cmd_a,pk);
	Function_not(ncmd_store,cmd_store,pk);
	Function_not(ncmd_cmp,cmd_cmp,pk);

    for(i=0;i<8;i++)
    {
    	Function_and2(t1,b1[i],ncmd_a,pk);
    	Function_and2(t2,load_arg[i], cmd_a,pk);
    	Function_or2(t3,t1,t2,pk);
		Function_not(b_cmp[i],t3,pk);
    }

    if(debug) dump("!cmp",b_cmp,8,NULL,sk);
    ALU_add(b_cmp,one_field, VALUE_ZERO,b_cmp_0,pk);
    copyreg(b_cmp,b_cmp_0);
    if(debug) dump("-cmp",b_cmp,8,NULL,sk);
    if(debug) dump("  b1",b1,8,NULL,sk);
    ALU_add(b_cmp,ac,VALUE_ZERO,b_cmp_0,pk);
    copyreg(b_cmp,b_cmp_0);
    if(debug) dump("cmp",b_cmp,8,NULL,sk);

    //rol b21
    fhe_int_set(carry_rol,ac[0]);
    for(i=0;i<7;i++)
		fhe_int_set(b_rol[i],ac[i+1]);
    fhe_int_set(b_rol[7],ALU_carry);
    if(debug) dump("rol",b_rol,8,carry_rol,sk);

    //ror b22
    fhe_int_set(carry_ror,ac[7]);
    for(i=7;i>0;i--)
		fhe_int_set(b_ror[i],ac[i-1]);
    fhe_int_set(b_ror[0],ALU_carry);
    if(debug) dump("ror",b_ror,8,carry_ror,sk);

    //add b31
    ALU_add(ac,b1,ALU_carry,temp31,pk);
    ALU_add(ac,load_arg,ALU_carry,temp32,pk);

    for(i=0;i<8;i++)
    {
    	Function_and2(t1,temp31[i],ncmd_a,pk);
    	Function_and2(t2,temp32[i],cmd_a,pk);
    	Function_or2(t3,t1,t2,pk);
		fhe_int_set(b_add[i],t3);
    }
    fhe_int_set(carry_add1,temp31[8]);
	fhe_int_set(carry_add2,temp32[8]);

	Function_and2(t1,carry_add1,ncmd_a,pk);
	Function_and2(t2,carry_add2,cmd_a,pk);
	Function_or2(carry_add,t1,t2,pk);
    if(debug) dump("add",b_add,8,carry_add,sk);

    //clc b32
    fhe_encrypt(carry_clc->v,pk,0);
    carry_clc->n=0;
    copyreg(b_clc,ac);
    if(debug) dump("clc",ac,8,carry_clc,sk);
    //sec b33
    fhe_encrypt(carry_sec->v,pk,1);
    carry_sec->n=0;
    copyreg(b_sec,ac);
    if(debug) dump("sec",ac,8,carry_sec,sk);

    //xor b41
    for(i=0;i<8;i++)
		Function_xor(b_xor[i],ac[i], b1[i],pk);
    if(debug) dump("xor",b_xor,8,ALU_carry,sk);
    //and b42
    for(i=0;i<8;i++)
		Function_and2(b_and[i],ac[i], b1[i],pk);
    if(debug) dump("and",b_and,8,ALU_carry,sk);
    //or b43
    for(i=0;i<8;i++)
		Function_or2(b_or[i],ac[i], b1[i],pk);
    if(debug) dump("or",b_or,8,ALU_carry,sk);

    //if(debug) System.out.println("cmd_load="+cmd_load+" cmd_store="+cmd_store+" cmd_ror="+cmd_ror+" cmd_rol="+cmd_rol+" cmd_add="+cmd_add+
    //                               " cmd_sec="+cmd_sec+" cmd_clc="+cmd_clc+" cmd_xor="+cmd_xor+" cmd_and="+cmd_and+" cmd_or="+cmd_or+
    //                               " cmd_beq="+cmd_beq+" cmd_jmp="+cmd_jmp+" cmd_la="+cmd_la+" cmd_bmi="+cmd_bmi+" cmd_cmp="+cmd_cmp);
    //if(debug) System.out.println("a="+cmd_a);
    //la

    if(debug){puts("PHASE 4: load/store");fflush(stdout);}
    //PHASE 4: load/store
    Memory_access2(b1, ac, ncmd_store,load_val,pk,sk);

    if(debug){puts("PHASE 5: rewrite registers");fflush(stdout);}
    //PHASE 5: rewrite registers / flags
    for(i=0;i<8;i++)
    {
		Function_and2(t1,b1[i], cmd_load,pk);
		Function_and2(t2,b_ror[i],cmd_ror,pk);
		Function_and2(t3,b_rol[i],cmd_rol,pk);
		Function_and2(t4,b_sec[i],cmd_sec,pk);
		Function_and2(t5,b_clc[i],cmd_clc,pk);
		Function_and2(t6,b_add[i],cmd_add,pk);
		Function_and2(t7,b_and[i],cmd_and,pk);
		Function_and2(t8,b_xor[i],cmd_xor,pk);
		Function_and2(t9,b_or[i],cmd_or,pk);
		Function_and2(t11,load_val[i],cmd_la,pk);
		Function_and2(t12,ac[i],cmd_beq,pk);
		Function_and2(t13,ac[i],cmd_bmi,pk);
		Function_and2(t14,ac[i],cmd_cmp,pk);
		Function_and2(t15,ac[i],cmd_jmp,pk);
		Function_or15(ac[i],t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,pk);
    }

	Function_not(t1,ac[0],pk);
	Function_not(t2,ac[1],pk);
	Function_not(t3,ac[2],pk);
	Function_not(t4,ac[3],pk);
	Function_not(t5,ac[4],pk);
	Function_not(t6,ac[5],pk);
	Function_not(t7,ac[6],pk);
	Function_not(t8,ac[7],pk);
	Function_and8(zero1,t1,t2,t3,t4,t5,t6,t7,t8,pk);


	Function_not(t1,b_cmp[0],pk);
	Function_not(t2,b_cmp[1],pk);
	Function_not(t3,b_cmp[2],pk);
	Function_not(t4,b_cmp[3],pk);
	Function_not(t5,b_cmp[4],pk);
	Function_not(t6,b_cmp[5],pk);
	Function_not(t7,b_cmp[6],pk);
	Function_not(t8,b_cmp[7],pk);
	Function_and8(zero2,t1,t2,t3,t4,t5,t6,t7,t8,pk);


	Function_and2(t1,zero2,cmd_cmp,pk);
	Function_and2(t2,ALU_zero,cmd_bmi,pk);
	Function_and2(t3,ALU_zero,cmd_beq,pk);
	Function_or3(t4,t1,t2,t3,pk);
	Function_and2(t5,zero1,ncmd_cmp,pk);
    Function_or2(ALU_zero,t5,t4,pk);


    Function_and2(t1,b_cmp[7], cmd_cmp,pk);
    Function_and2(t2,ALU_minus, ncmd_cmp,pk);
    Function_or2(ALU_minus,t2,t1,pk);


    Function_and2(t1,carry_add, cmd_add,pk);
    fhe_int_set(carry_add,t1);
    Function_and2(t1,carry_rol, cmd_rol,pk);
    fhe_int_set(carry_rol,t1);
    Function_and2(t1,carry_ror, cmd_ror,pk);
    fhe_int_set(carry_ror,t1);
    Function_and2(t1,carry_clc, cmd_clc,pk);
    fhe_int_set(carry_clc,t1);
    Function_and2(t1,carry_sec, cmd_sec,pk);
    fhe_int_set(carry_sec,t1);


    Function_not(t1,cmd_add,pk);
    Function_not(t2,cmd_rol,pk);
    Function_not(t3,cmd_ror,pk);
    Function_not(t4,cmd_clc,pk);
    Function_not(t5,cmd_sec,pk);
    Function_and4(t6,t1,t2,t3,t4,pk);
    Function_and3(carry_non,t6,t5,ALU_carry,pk);

    Function_or6(ALU_carry,carry_add,carry_rol,carry_ror,carry_clc,carry_sec,carry_non,pk);


    ALU_addadr(pc,one_field,pc_linear,pk);
    pc_jump=b1;
    pc_branch=b1;

    if(debug) dump("PC lin",pc_linear,8,NULL,sk);
    if(debug) dump("PC bra",pc_branch,8,NULL,sk);
    if(debug) dump("PC jmp",pc_jump,8,NULL,sk);

    if(debug)
    {
    	printf("ZERO=%d MINUS=%d\n",fhe_decrypt(ALU_zero->v,sk),fhe_decrypt(ALU_minus->v,sk));
    }

#define ALU_not_minus t2
#define ALU_not_zero t3
#define ncmd_jmp t4
#define ncmd_beq t5
#define ncmd_bmi t6
#define jmpaddr t1
#define bmiplusaddr t7
#define beqnoeqaddr t8
#define nojmpaddr t9
#define bmiminusaddr t10
#define beqequaladdr t11

	Function_not(ncmd_jmp,cmd_jmp,pk);
	Function_not(ALU_not_minus,ALU_minus,pk);
	Function_not(ncmd_bmi,cmd_bmi,pk);
	Function_not(ALU_not_zero,ALU_zero,pk);
	Function_not(ncmd_beq,cmd_beq,pk);

	printf("ALUnz:%d ALUnm:%d njmp:%d nbmi:%d nbeq:%d\n",
			fhe_decrypt(ALU_not_zero->v,sk),
			fhe_decrypt(ALU_not_minus->v,sk),
			fhe_decrypt(ncmd_jmp->v,sk),
			fhe_decrypt(ncmd_bmi->v,sk),
			fhe_decrypt(ncmd_beq->v,sk));

    for(i=0;i<Memory_ARRAY_COLS;i++)
    {
    	//Function_and2(t1,cmd_jmp,pc_jump[i],pk);
    	Function_and2(jmpaddr,cmd_jmp,pc_jump[i],pk);
		Function_and3(bmiplusaddr,cmd_bmi,ALU_not_minus,pc_linear[i],pk);
		Function_and3(beqnoeqaddr,cmd_beq,ALU_not_zero,pc_linear[i],pk);
		Function_and4(nojmpaddr,ncmd_bmi,ncmd_beq,ncmd_jmp,pc_linear[i],pk);
		Function_and3(bmiminusaddr,cmd_bmi,pc_branch[i],ALU_minus,pk);
		Function_and3(beqequaladdr,cmd_beq,pc_branch[i],ALU_zero,pk);
		Function_or3(t12,beqequaladdr,bmiminusaddr,nojmpaddr,pk);
		Function_or4(pc[i],t12,beqnoeqaddr,bmiplusaddr,jmpaddr,pk);
//    	Function_and3(t7,cmd_bmi,ALU_not_minus,pc_linear[i],pk);
//    	Function_and3(t8,cmd_beq,ALU_not_zero,pc_linear[i],pk);
//    	Function_and3(t9,ncmd_bmi,ncmd_beq,ncmd_jmp,pk);
//    	Function_and3(t10,cmd_bmi,pc_branch[i],ALU_minus,pk);
//    	Function_and3(t11,cmd_beq,pc_branch[i],ALU_zero,pk);
//    	Function_or3(t12,t11,t10,t9,pk);
//      Function_or4(pc[i],t12,t8,t7,t1,pk);
    	//Function_or2(pc[i],t1,VALUE_ZERO,pk);
    }

    if(debug) dump("PC",pc,8,NULL,sk);
    if(debug) dump("AC",ac,8,ALU_carry,sk);
    if(debug) printf("zero=%d\n",fhe_decrypt(ALU_zero->v,sk));

    if(debug) puts("____END_PROCESS____");

	return 0;
}

void dump(char *cmd,fhe_int_t *reg,int length, fhe_int_t carry,fhe_sk_t sk)
{
    int i;
    int max=13;
    if(length<max)
        max=length;

    printf("(%d) %s:",dumpid++,cmd);
    for(i=0;i<max;i++)
        printf("%d ",fhe_decrypt(reg[i]->v,sk));
    if(carry!=NULL)
        printf(" carry=%d\n",fhe_decrypt(carry->v,sk));
    else
        puts("");
}

void copyreg(fhe_int_t *a,fhe_int_t *b)
{
    int i;
    for(i=0;i<8;i++)
		fhe_int_set(a[i],b[i]);
}



int decode(int *adr,int length)
{
	int d=0;
	int mask=1;
	int i;

	for(i=0;i<length;i++)
	{
		if((adr[i]&1)==1)
			d+=mask;
		mask*=2;
	}
	//		System.out.println("decode="+d);
	return d;
}



int main2(int argc, char **argv)
{
	int i;
	clock_t start_t,cycle0_t,end_t,cycle_s,cycle_e,cycle_avg;

	start_t=clock();

	//XXX loadMemory(ac,pc);

	//		memdump(2);
	//		int reg[]=new int[]{1,0,1,0,1,0,1,0,1,0,1,0};
	//		int r2[]=Memory.access(new int[]{0,0,0,0,0}, reg, 1);
	//
	//		for(int i=0;i<8;i++)
	//			System.out.println(r2[i]);
	//
	//		memdump(2);

	//		memdump(5);

//	Function_opcountand=0;
//	Function_opcountnot=0;
//	Function_opcountxor=0;

	//if(debug) memdump(8);
	cycle_avg=0;
	cycle0_t=clock();
	for(i=0;i<115;i++)
	{
		if(debug) printf("CYCLE %d\n",i);

		cycle_s=clock();
		//CU_process();
		cycle_e=clock()-cycle_s;
		cycle_avg+=cycle_e;
		//			System.out.println("cycle time="+cycletime);

		//			if(decode(CU.pc)==49)
		//				System.out.println("end after cycle "+i);
	}
	//systime=System.currentTimeMillis()-systime;
	//		CU.dump("AC", CU.ac, ALU.carry);
	end_t=clock();
	//if(debug) memdump(133);

//	printf("and:%d\n",Function_opcountand);
//	printf("xor:%d\n",Function_opcountxor);
//	printf("not:%d\n",Function_opcountnot);

	printf("%d %d %d (avg. cycle=%ld)\n",(int)start_t,(int)cycle0_t,(int)end_t,cycle_avg/115);

	//System.out.println("time="+systime+"ms");
	//System.out.println("avg. cycletime="+cycletime/238+"ms");
	return 0;
}

int main(int argc,char **argv)
{
	time_t tme,tme2;
	fhe_pk_t pk;
	fhe_sk_t sk;
	fhe_int_t rw,a[8],b[8],res[9],c,memword[Memory_WORD_SIZE],memaddr[Memory_ARRAY_COLS];
	int i,m,runs,rmin,rmax,rsum;

	puts("init");fflush(stdout);
	for(i=0;i<8;i++)
	{
		fhe_int_init(a[i]);
		fhe_int_init(b[i]);
		fhe_int_init(res[i]);
	}
	for(i=0;i<Memory_WORD_SIZE;i++)
		fhe_int_init(memword[i]);
	for(i=0;i<Memory_ARRAY_COLS;i++)
		fhe_int_init(memaddr[i]);

	fhe_int_init(rw);
	fhe_int_init(c);
	fhe_int_init(res[8]);
	fhe_sk_init(sk);
	fhe_pk_init(pk);
	fhe_lib_init(&sk);

//	puts("keygen");fflush(stdout);
//	fhe_keygen(pk,sk);

	puts("loadkey");
	fhe_pk_loadkey(pk,"/home/micha/workspace/smart-fhe-kit/mykey.pk");
	fhe_sk_loadkey(sk,"/home/micha/workspace/smart-fhe-kit/mykey.sk");

	puts("and");
	fhe_encrypt(a[0]->v,pk,0);
	fhe_encrypt(a[1]->v,pk,1);

	Function_and2(c,a[0],a[0],pk);
	printf("%d AND %d = %d(%d)\n",0,0,fhe_decrypt(c->v,sk),c->n);

	Function_and2(c,a[0],a[1],pk);
	printf("%d AND %d = %d(%d)\n",0,1,fhe_decrypt(c->v,sk),c->n);

	Function_and2(c,a[1],a[0],pk);
	printf("%d AND %d = %d(%d)\n",1,0,fhe_decrypt(c->v,sk),c->n);

	Function_and2(c,a[1],a[1],pk);
	printf("%d AND %d = %d(%d)\n",1,1,fhe_decrypt(c->v,sk),c->n);

	puts("encode");fflush(stdout);
	fhe_encrypt(c->v,pk,1);
	Function_encode(8,255,a,pk);
	Function_encode(8,1,b,pk);

	printf("  a = %d\n",Function_decode2(a,8,sk));
	printf("  b = %d\n",Function_decode2(b,8,sk));

	puts("add");fflush(stdout);
	ALU_add(a,b,c,res,pk);
	printf("  res = %d + %d\n",Function_decode2(res,8,sk),fhe_decrypt(res[8]->v,sk));

	puts("addadr");fflush(stdout);
	ALU_addadr(a,b,res,pk);
	printf("  res = %d\n",Function_decode2(res,8,sk));

	puts("loadmem");fflush(stdout);
	loadMemory(a,b,"/home/micha/workspace/mlee_assembler/test.mleeobj");

	printf("  ac = %d\n",Function_decode2(a,8,sk));
	printf("  pc = %d\n",Function_decode2(b,8,sk));

	memdump(4,sk);

//	puts("CU_init");fflush(stdout);
	CU_init(pk);
//	puts("CU_process");fflush(stdout);
//	CU_process(pk,sk);
//	CU_process(pk,sk);
//	CU_process(pk,sk);
//
//	printf("fhe errors: %d\n",geterrors());
//	memdump(4,sk);
//
//	if(debug)
//		return 0;

	puts("perform access");fflush(stdout);

//	fhe_encrypt(memword[0]->v,pk,1);
//	fhe_encrypt(memword[2]->v,pk,1);
//	fhe_encrypt(memword[7]->v,pk,1);

	rmax=0;rmin=999999999;rsum=0;
	for(runs=0;runs<4;runs++)
	{
		Function_encode(8,runs,memaddr,pk);
		Function_encode(13,0,memword,pk);

		dump("adr",memaddr,8,NULL,sk);
		time(&tme);
		Memory_access2(memaddr,res,Memory_READ,memword,pk,sk);


	//	Function_encode(8,3,memaddr,pk);
	//	Memory_access2(memaddr,res,rw,memword,pk,sk);

		time(&tme2);
		//puts("\nok");fflush(stdout);
		dump("mem",memword,13,NULL,sk);
	//	m=Function_decode2(memword,13,sk);
	//	for(i=0;i<13;i++)
	//		printf("%d ",fhe_decrypt(memword[i]->v,sk));
		printf("%d s\n",tme2-tme);
		if((tme2-tme)>rmax)
			rmax=tme2-tme;
		if((tme2-tme)<rmin)
			rmin=tme2-tme;
		rsum+=(tme2-tme);
	}
	printf("min=%d max=%d avg=%d\n",rmin,rmax,rsum/10);
	memdump(4,sk);
}
