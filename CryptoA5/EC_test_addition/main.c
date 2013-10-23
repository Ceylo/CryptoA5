//
//  main.c
//  EC_tests
//
//  Created by Baptiste on 10/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include <gmp.h>
#include "ec_operations.h"
#include <assert.h>

//(2,4) (3,1) a4 = 1, p = 5
static void addPoints(PointRef p, PointRef q, CurveRef curve);

//(3,2) (3,2) a4 = 1, p = 5
static void addDoublePoint(PointRef p, PointRef q, CurveRef curve);

//p = teta
static void addPteta(PointRef p, PointRef q, CurveRef curve);

//q = teta
static void addQteta(PointRef p, PointRef q, CurveRef curve);

//p = -q
static void addResultTeta(PointRef p, PointRef q, CurveRef curve);


int main(int argc, const char * argv[])
{
    mpz_t a[7], mod, n;
    mpz_inits(a[0], a[1], a[2], a[3], a[4], a[5], a[6], n, NULL);
    mpz_init_set_si(mod, 5);
    mpz_set_si(a[4], 1);
    mpz_set_si(n, 5);
    
    PointRef g = PointCreateFromInt(0,1);
    PointRef p = PointCreateFromInt(2,4);
    PointRef q = PointCreateFromInt(3,1);
    PointRef pTeta = PointCreateTeta();
    PointRef qTeta = PointCreateTeta();
    PointRef p2 = PointCreateFromInt(3,2);
    PointRef p2Inv = PointCreateFromInt(3,3);

    
    CurveRef curve = CurveCreate(mod, n, a, g);
    
    assert(curve != NULL);
	assert(curve->a[4] != NULL);
	assert(curve->a[6] != NULL);
	assert(curve->g != NULL);
	assert(curve->mod != NULL);
    
    addPoints(p, q, curve);
    
    addDoublePoint(p2, p2, curve);
    
    addPteta(pTeta, q, curve);
    
    addQteta(p, qTeta, curve);
    
    addResultTeta(p2, p2Inv, curve);
    
    mpz_clears(mod, a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    PointDestroy(g);
    PointDestroy(p);
    PointDestroy(q);
    PointDestroy(pTeta);
    PointDestroy(qTeta);
    PointDestroy(p2);
    PointDestroy(p2Inv);
    
    CurveDestroy(curve);

    return 0;
}

static void addPoints(PointRef p, PointRef q, CurveRef curve) {
    PointRef r = PointCreateAdd(p, q, curve);
    
	assert(mpz_cmp_si(r->x, 4) == 0);
	assert(mpz_cmp_si(r->y, 2) == 0);
    
    assert(PointIsOnCurve(r, curve));
    
    PointDestroy(r);
}

static void addDoublePoint(PointRef p, PointRef q, CurveRef curve) {
    PointRef r = PointCreateAdd(p, q, curve);
    
	assert(mpz_cmp_si(r->x, 3) == 0);
	assert(mpz_cmp_si(r->y, 3) == 0);
    
    PointDestroy(r);
}

static void addPteta(PointRef p, PointRef q, CurveRef curve) {
    PointRef r = PointCreateAdd(p, q, curve);
    
	assert(mpz_cmp_si(r->x, 3) == 0);
	assert(mpz_cmp_si(r->y, 1) == 0);
    
    PointDestroy(r);
}

static void addQteta(PointRef p, PointRef q, CurveRef curve) {
    PointRef r = PointCreateAdd(p, q, curve);
    
	assert(mpz_cmp_si(r->x, 2) == 0);
	assert(mpz_cmp_si(r->y, 4) == 0);
    
    PointDestroy(r);
}

static void addResultTeta(PointRef p, PointRef q, CurveRef curve) {
    PointRef r = PointCreateAdd(p, q, curve);
    
	assert(mpz_cmp_si(r->x, 0) == 0);
	assert(mpz_cmp_si(r->y, 0) == 0);
    assert(r->infinite == true);
    
    PointDestroy(r);
}

