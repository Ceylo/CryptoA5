//
//  ec_operations.h
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef CryptoA5_ec_operations_h
#define CryptoA5_ec_operations_h

#include <gmp.h>
#include <stdbool.h>

typedef struct {
	mpz_t mod;
} Field;

typedef Field* FieldRef;

typedef struct {
	FieldRef field;
	mpz_t x;
	mpz_t y;
} Point;

typedef Point* PointRef;


FieldRef	FieldCreate(int mod);
void		FieldDestroy(FieldRef field);
bool		FieldEqual(FieldRef aField, FieldRef anotherField);

PointRef	PointCreate(FieldRef field);
void		PointDestroy(PointRef point);
bool		PointEqual(PointRef p, PointRef q);

PointRef	PointCreateInvert(PointRef p);
PointRef	PointCreateAdd(PointRef p, PointRef q);
PointRef	PointCreateDouble(PointRef p);
PointRef	PointCreateMultiple(PointRef p, int scalar);

#endif
