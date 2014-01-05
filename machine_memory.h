/*
 * memory.h
 *
 *  Created on: 20.01.2011
 *      Author: micha
 */
#pragma once
#ifndef MACHINE_MEMORY_H_
#define MACHINE_MEMORY_H_

#include <gmp.h>
#include "integer-fhe.h"

static int debug=1;


#define Memory_WORD_SIZE 13
#define Memory_ARRAY_ROWS 256
#define Memory_ARRAY_COLS 8
fhe_int_t Memory_cellarray[Memory_ARRAY_ROWS][Memory_WORD_SIZE];

fhe_int_t Memory_READ; //=1

void ALU_add(fhe_int_t *a,fhe_int_t *b,fhe_int_t carry,fhe_int_t *into,fhe_pk_t pk);
void ALU_addadr(fhe_int_t *a,fhe_int_t *b,fhe_int_t *into,fhe_pk_t pk);

//static int Memory_WRITE=42;
void Memory_access(fhe_int_t *a,fhe_int_t *reg,fhe_int_t rw,fhe_int_t *into,fhe_pk_t pk,fhe_sk_t sk);
void Memory_access2(fhe_int_t *a,fhe_int_t *reg,fhe_int_t rw,fhe_int_t *into,fhe_pk_t pk,fhe_sk_t sk);
void memdump(int rows,fhe_sk_t sk);
void loadMemory(fhe_int_t *ac,fhe_int_t *pc,char *filename);

#endif /* MEMORY_H_ */
