//
//  ec_operations.h
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef CryptoA5_ec_operations_h
#define CryptoA5_ec_operations_h

#include <stdio.h>
#include <stdarg.h>
#include <gmp.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	mpz_t x;
	mpz_t y;
	bool infinite;
} Point;
typedef Point*	PointRef;

typedef struct {
	mpz_t mod;
	mpz_t a[7];
	mpz_t n;
	PointRef g;
} Curve;
typedef Curve*	CurveRef;


CurveRef		CurveCreate(mpz_t mod, mpz_t n, mpz_t a[7], PointRef g);
CurveRef		CurveCreateFromData(const char *data);
CurveRef		CurveCreateFromFile(const char *filename);
void			CurveDestroy(CurveRef curve);
bool			CurveEqual(CurveRef aCurve, CurveRef anotherCurve);

PointRef		PointCreateTeta();
bool			PointIsTeta(PointRef p);
PointRef		PointCreate();
PointRef		PointCreateFromGMP(mpz_t x, mpz_t y);
PointRef		PointCreateFromInt(int x, int y);

// Create a textual representation of the point.
// You're RESPONSIBLE for FREEING the returned string.
char *			PointCreateDescription(PointRef p);
PointRef		PointCopy(PointRef other);
void			PointDestroy(PointRef point);
bool			PointEqual(PointRef p, PointRef q);
bool			PointCongruent(PointRef p, PointRef q, CurveRef curve);

PointRef		PointCreateInvert(PointRef p, CurveRef curve);
PointRef		PointCreateAdd(PointRef p, PointRef q, CurveRef curve);
PointRef		PointCreateDouble(PointRef p, CurveRef curve);
PointRef		PointCreateMultiple(PointRef p, mpz_t scalar, CurveRef curve);
bool            PointIsOnCurve(PointRef p, CurveRef curve);

#ifdef __cplusplus
}
#endif

#endif
