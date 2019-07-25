/*
 * memory.h
 *
 *  Created on: 20.01.2011
 *      Author: micha
 */
#pragma once
#ifndef MACHINE_MEMORY_H_
#define MACHINE_MEMORY_H_

//#include <gmp.h>
#include "integer-fhe.h"
#include "types2.h"

static int debug=1;


#define Memory_WORD_SIZE 13
#define Memory_ARRAY_ROWS 256
#define Memory_ARRAY_COLS 8
int Memory_cellarray[Memory_ARRAY_ROWS][Memory_WORD_SIZE];

int Memory_READ; //=1

void ALU_add(int *a,int *b,int carry,int *into,int pk);
void ALU_addadr(int *a,int *b,int *into,int pk);

//static int Memory_WRITE=42;
void Memory_access(int *a,int *reg,int rw,int *into,int pk,int sk);
void Memory_access2(int *a,int *reg,int rw,int *into,int pk,int sk);
void memdump(int rows,int sk);
void loadMemory(int *ac,int *pc,char *filename);

#endif /* MEMORY_H_ */
