/*
 *  types.h
 *  integer-fhe
 *
 *  Created by Henning Perl on 25.11.10.
 *  Copyright 2010 Henning Perl. All rights reserved.
 *
 */

#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

#include "parameters.h"

/** type defs **/
typedef struct {
	int p, alpha;
	int c[FHE_S1], B[FHE_S1];
} _fhe_pk;
typedef _fhe_pk fhe_pk_t[1];

typedef struct {
	int p, B;
} _fhe_sk;
typedef _fhe_sk fhe_sk_t[1];

/** memory management **/

void fhe_pk_init(int pk);

void fhe_pk_clear(int pk);

void fhe_sk_init(int sk);

void fhe_sk_clear(int sk);

/** output **/

void fhe_pk_print(int pk);

void fhe_sk_print(int sk);

#endif
