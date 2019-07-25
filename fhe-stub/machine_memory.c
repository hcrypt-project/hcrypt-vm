/*
 * memory.c
 *
 *  Created on: 20.01.2011
 *      Author: micha
 */

/*
 * Memory.c
 *
 *  Created on: 30.07.2010
 *      Author: root
 */

/*
 * This class implements encrypted memory access.
 *
 * input
 * -----
 * cellarray    is an array of encrypted memory bit values
 * a                    is the encrypted memory address
 *
 * output
 * ------
 * b                    is the encrypted word at the given address
 *
 * Access is realized as a series of 1-out-of-n-bit boolean data selectors
 *
 * Example for 3 address lines and 7 single-column memory rows:
 *
 * ------and-circuit---->
 *
 * o= !a0 & !a1 & !a2 & m0 | \          |
 *     a0 & !a1 & !a2 & m1 | \          |
 *    !a0 &  a1 & !a2 & m2 | \          |
 *     a0 &  a1 & !a2 & m3 | \          or-circuit
 *    !a0 & !a1 &  a2 & m4 | \          |
 *     a0 & !a1 &  a2 & m5 | \          |
 *    !a0 &  a1 &  a2 & m6 | \          |
 *     a0 &  a1 &  a2 & m7              \/
 *
 * Writing to a cell applies the following assumption: after read or write
 * access, the register and the cell have the same values, so the function
 * for a cell-bit and a register-bit in dependency to a READ/WRITE signal
 * is:
 *
 * reg=cell= (cell & WRITE) | (reg & READ)
 *
 * this can be derived from the following truth table
 *
 * R = register bit
 * M = memory (cell bit)
 * W = write signal (inverse common circuit logic: 0=read 1=write)
 *
 * R M W  R' M'
 * ------------
 * 0 0 0  0  0
 * 0 0 1  0  0
 * 0 1 0  1  1
 * 0 1 1  0  0
 * 1 0 0  0  0
 * 1 0 1  1  1
 * 1 1 0  1  1
 * 1 1 1  1  1
 *
 * this can be simplified to R'=M'=(C and !W) or (R and W)
 *
 * assuming that the cell has been selected (and-circuit)
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include "machine_memory.h"
#include "machine_function.h"
#include "types.h"

int r[Memory_ARRAY_ROWS];
int nr[Memory_ARRAY_ROWS];
int rowsel;
int nam,nam2,nrw,read,write,nsel,ZERO;
static int inited=0;

void Memory_access(int *a,int *reg,int rw,int *b1,int pk,int sk)
{
	int row;
	int mask;
	int m;
	int i;
	int i2;
	int j;


	if(!inited)
	{
		inited=1;
		for(i=0;i<Memory_ARRAY_ROWS;i++)
			fhe_int_init(r[i]);

		fhe_int_init(nam);
		fhe_int_init(nam2);
		fhe_int_init(nrw);
		fhe_int_init(read);
		fhe_int_init(write);
		fhe_int_init(nsel);
	}

	puts("memory access 1");fflush(stdout);
	//this loop generates a positive row signal for the *one* selected row
	if(fhe_decrypt(rw->v,sk)==1)
		puts("  read request");
	else
		puts("  write request");

	for(row=0;row<Memory_ARRAY_ROWS;row++)
	{
		mask=4;
		Function_not(nam,a[0],pk);
		Function_not(nam2,a[1],pk);
		Function_and2(r[row],(row&1)>0?a[0]:nam,(row&2)>0?a[1]:nam2,pk);

		for(m=2;m<Memory_ARRAY_COLS;m++,mask<<=1)
		{
			Function_not(nam,a[m],pk);
			Function_and2(r[row],r[row],(row&mask)>0?a[m]:nam,pk);
		}
		if(fhe_decrypt(r[row]->v,sk)==1)
			printf("  row selected: %d\n",row);
	}


	puts("memory access 2");fflush(stdout);
	//now we generate a
	for(row=0;row<Memory_ARRAY_ROWS;row++)
	{
		for(i2=0;i2<Memory_ARRAY_COLS;i2++)
		{
			Function_not(nam,r[row],pk);
			Function_not(nrw,rw,pk);

			Function_and2(write,nrw,reg[i2],pk); //write bit
			Function_and2(read,rw,Memory_cellarray[row][i2],pk); //read bit
			Function_and2(nsel,nam,Memory_cellarray[row][i2],pk); //row not selected
			Function_or3(Memory_cellarray[row][i2],read,write,nsel,pk);
		}
	}

	puts("memory access 3");fflush(stdout);
	//combine row signals and cell bits (or-circuit)
	//load b1
	for(i=0;i<Memory_WORD_SIZE;i++)
	{
		Function_and2(nam ,r[0],Memory_cellarray[0][i],pk);
		Function_and2(nam2,r[1],Memory_cellarray[1][i],pk);
		Function_or2 (b1[i],nam,nam2,pk);

		//b1[i]=Function_or2(Function_and2(r[0],Memory_cellarray[0][i]),Function_and2(r[1],Memory_cellarray[1][i]));
		for(j=2;j<Memory_ARRAY_ROWS;j++)
		{
			Function_and2(nam,r[j],Memory_cellarray[j][i],pk);
			Function_or2(b1[i],b1[i],nam,pk);

			//b1[i]=Function_or2(b1[i],Function_and2(r[j],Memory_cellarray[j][i]));
		}
	}
	return;
}

void Memory_access2(int *a,int *reg,int rw,int *b1,int pk,int sk)
{
	int row;
	int mask;
	int m;
	int i;
	int i2;
	int j;
	int b2[Memory_WORD_SIZE];


	if(!inited)
	{
		inited=1;

		fhe_int_init(rowsel);
		fhe_int_init(nam);
		fhe_int_init(nam2);
		fhe_int_init(nrw);
		fhe_int_init(read);
		fhe_int_init(write);
		fhe_int_init(nsel);
		fhe_int_init(ZERO);
		fhe_encrypt(ZERO->v,pk,0);
		for(i=0;i<Memory_ARRAY_ROWS;i++)
			fhe_int_init(r[i]);
	}
//	for(i=0;i<Memory_WORD_SIZE;i++)
//		fhe_int_init(b2[i]);

//	puts("memory access 1");fflush(stdout);
	//this loop generates a positive row signal for the *one* selected row
//	if(fhe_decrypt(rw->v,sk)==1)
//		puts("  read request");
//	else
//		puts("  write request");

	for(i=0;i<13;i++)
	{
		fhe_int_init(b2[i]);
		fhe_int_set(b2[i],ZERO);
	}

	Function_not(nrw,rw,pk);

	for(row=0;row<4/*Memory_ARRAY_ROWS*/;row++)
	{
		/* SCHRITT 1:
		 *
		 * fuer jede row ein select-signal generieren
		 *
		 * dieser abschnitt implementiert die AND-logik eines
		 * selektor-schaltkreises; das vielstellige AND wird
		 * durch viele zweistellige ANDs umgesetzt:
		 * AND(a,b,c,d) = AND(AND(AND(a,b),c),d)
		 *
		 * es wird durch die binaeren darstellungen der row-nummer
		 * geritten und mit der angelegten adresse a[0..COLS] "verglichen":
		 *
		 * row[1]=row[1000]= a[0] & !a[1] & !a[2] & !a[3]
		 * row[5]=row[1010]= a[0] & !a[1] &  a[2] & !a[3]
		 *
		 * am ende enthaelt nur eine row die 1 (sofern mit einer gueltigen
		 * adresse zugegriffen wird)
		 *
		 */

		mask=4;
		//das "innere" AND von hand...
		Function_not(nam,a[0],pk);
		Function_not(nam2,a[1],pk);
		Function_and2(r[row],(row&1)>0?a[0]:nam,(row&2)>0?a[1]:nam2,pk);

		// ...die weiteren ANDs werden "aufaddiert"
		for(m=2;m<Memory_ARRAY_COLS;m++,mask<<=1)
		{
			Function_not(nam,a[m],pk);
			Function_and2(r[row],r[row],(row&mask)>0?a[m]:nam,pk);
		}
		Function_not(nam,r[row],pk);

		/*
		 * SCHRITT 2:
		 *
		 * speicherzelle mit select-signal verknuepfen; dieser abschnitt
		 * implementiert den zweiten teil der AND-logik
		 *
		 */
		for(m=0;m<Memory_WORD_SIZE;m++)
		{
			Function_and2(read,Memory_cellarray[row][m],r[row],pk);
			Function_and2(write,b1[m],r[row],pk);

			/*
			 * SCHRITT 3:
			 *
			 * refresh der speicherzellen des data-compartments
			 * (alter wert wenn row-select=0, sonst neuer wert aus register)
			 */
			if(m<8)
			{
				//set memory cell
				Function_and3(read,Memory_cellarray[row][m],r[row],rw,pk);
				Function_and3(write,b1[m],r[row],nrw,pk);
				Function_and2(nsel,Memory_cellarray[row][m],nam,pk);
				Function_or3(Memory_cellarray[row][m],read,write,nsel,pk);
			}
			/*
			 * SCHRITT 4:
			 *
			 * register schreiben; dies ist die OR-logik des selektor-
			 * schaltkreises
			 */
			Function_or3(b2[m],b2[m],read,write,pk);
		}
	}

	for(m=0;m<13;m++)
	{
		fhe_int_set(b1[m],b2[m]);
		fhe_int_clear(b2[m]);
	}

	return;
}

void ALU_add(int *a,int *b,int carry,int *res,int pk)
{
	int t[2];
	int c;
	int i;

	fhe_int_init(t[0]);
	fhe_int_init(t[1]);
	fhe_int_init(c);
	mpz_set(c->v,carry->v);
	c->n=carry->n;

	for(i=0;i<8;i++)
	{
		Function_fa(a[i], b[i], c,t,pk);
		fhe_int_set(res[i],t[0]);
		fhe_int_set(c,t[1]);
	}
	fhe_int_set(res[8],c);
	fhe_int_clear(t[0]);
	fhe_int_clear(t[1]);
	fhe_int_clear(c);
}

void ALU_addadr(int *a,int *b,int *res,int pk)
{
	int t[2];
	int c;
	int i;

	fhe_int_init(t[0]);
	fhe_int_init(t[1]);
	fhe_int_init(c);

	fhe_encrypt(c->v,pk,0);

	for(i=0;i<Memory_ARRAY_COLS;i++)
	{
		Function_fa(a[i], b[i], c,t,pk);
		fhe_int_set(res[i],t[0]);
		fhe_int_set(c,t[1]);
	}
	fhe_int_clear(t[0]);
	fhe_int_clear(t[1]);
	fhe_int_clear(c);
}


void loadMemory(int *ac,int *pc,char *filename)
{
	int i,j;
	FILE *f;
	char s[16384];

	for(i=0;i<Memory_ARRAY_ROWS;i++)
		for(j=0;j<Memory_WORD_SIZE;j++)
			fhe_int_init(Memory_cellarray[i][j]);

	f=fopen(filename,"r");

	i=0;
	while(!feof(f))
	{
		fgets(s,16384,f);
		if(feof(f))
			break;

		if(i<8)
			mpz_set_str(ac[i]->v,s,10);
		else if(i<16)
			mpz_set_str(pc[i-8]->v,s,10);
		else
		{
//			if((i-16)%13==0)
//				puts("read word");
//			printf("read bit at %d,%d (%d)\n",(i-16)/Memory_WORD_SIZE,(i-16)%Memory_WORD_SIZE,i-16);

			mpz_set_str(Memory_cellarray[(i-16)/Memory_WORD_SIZE][(i-16)%Memory_WORD_SIZE]->v,s,10);
		}
		i++;
	}
	fclose(f);
}

void memdump(int rows,int sk)
{
    int i,j;
	int shift,val;

	puts("___DUMP_START___");
	for(i=0;i<rows;i++)
	{
        shift=1;val=0;
		printf("%d\t",i);
		for(j=0;j<Memory_WORD_SIZE;j++)
		{
			if(j<8)
				if(fhe_decrypt(Memory_cellarray[i][j]->v,sk)==1)
					val+=shift;
			shift*=2;

			printf("%d ",fhe_decrypt(Memory_cellarray[i][j]->v,sk));
		}
		printf(" %d\n",val);
	}
	puts("___DUMP_END_____");
}
