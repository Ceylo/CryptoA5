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
	mpz_t x;
	mpz_t y;
} Point;
typedef Point* PointRef;

typedef struct {
	mpz_t mod;
	int a[7];
	PointRef g;
} Curve;
typedef Curve* CurveRef;


CurveRef	CurveCreate(mpz_t mod, int a[7], PointRef g);
void		CurveDestroy(CurveRef curve);
bool		CurveEqual(CurveRef aCurve, CurveRef anotherCurve);

PointRef	PointCreate();
PointRef	PointCreateFromGMP(mpz_t x, mpz_t y);
PointRef	PointCreateFromInt(int x, int y);
PointRef	PointCopy(PointRef other);
void		PointDestroy(PointRef point);
bool		PointEqual(PointRef p, PointRef q);
bool		PointCongruent(PointRef p, PointRef q, CurveRef curve);

PointRef	PointCreateInvert(PointRef p, CurveRef curve);
PointRef	PointCreateAdd(PointRef p, PointRef q, CurveRef curve);
PointRef	PointCreateDouble(PointRef p, CurveRef curve);
PointRef	PointCreateMultiple(PointRef p, int scalar, CurveRef curve);

#endif
