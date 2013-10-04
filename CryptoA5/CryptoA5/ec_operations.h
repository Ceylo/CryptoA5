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

typedef struct {
	mpz_t x;
	mpz_t y;
} Point;

typedef Point* PointRef;

PointRef	PointCreate();
void		PointDestroy(PointRef point);
int			PointCompare(PointRef p, PointRef q);

PointRef	PointCreateInvert(PointRef p);
PointRef	PointCreateAdd(PointRef p, PointRef q);
PointRef	PointCreateDouble(PointRef p);
PointRef	PointCreateMultiple(PointRef p, int scalar);

#endif
