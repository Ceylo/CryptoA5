//
//  main.c
//  EC_test_invert
//
//  Created by Baptiste on 10/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "ec_operations.h"
#include <assert.h>

//inv p
static void invP(PointRef p, CurveRef curve);

//p = teta
static void invPteta(PointRef p, CurveRef curve);

int main(int argc, const char * argv[])
{
    mpz_t a[7], mod, n;
    mpz_inits(a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    mpz_init_set_si(mod, 5);
	mpz_init_set_si(n, 5 * 5);
    mpz_set_si(a[4], 1);
    
    PointRef g = PointCreateFromInt(0,1);
    PointRef p = PointCreateFromInt(3,3);
    PointRef pTeta = PointCreateTeta();

    
    CurveRef curve = CurveCreate(mod, n, a, g);
    
    assert(curve != NULL);
	assert(curve->a[4] != NULL);
	assert(curve->a[6] != NULL);
	assert(curve->g != NULL);
	assert(curve->mod != NULL);
    
    invP(p, curve);
    
    invPteta(pTeta, curve);
    
    mpz_clears(mod, n, a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    PointDestroy(g);
    PointDestroy(p);
    PointDestroy(pTeta);
    
    CurveDestroy(curve);
    
    return 0;
}

static void invP(PointRef p, CurveRef curve) {
    PointRef r = PointCreateInvert(p, curve);
    
	assert(mpz_cmp_si(r->x, 3) == 0);
	assert(mpz_cmp_si(r->y, 2) == 0);
    
    PointDestroy(r);
}

static void invPteta(PointRef p, CurveRef curve) {
    PointRef r = PointCreateInvert(p, curve);

    assert(mpz_cmp_si(r->x, 0) == 0);
	assert(mpz_cmp_si(r->y, 0) == 0);
    assert(r->infinite == true);
    
    PointDestroy(r);

}



