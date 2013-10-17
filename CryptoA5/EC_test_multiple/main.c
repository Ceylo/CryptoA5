//
//  main.c
//  EC_test_multiple
//
//  Created by Baptiste on 10/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "ec_operations.h"
#include <assert.h>

int main(int argc, const char * argv[])
{
    mpz_t a[7], mod;
    mpz_inits(a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    mpz_init_set_si(mod, 5);
    mpz_set_si(a[4], 1);
    
    PointRef g = PointCreateFromInt(0,1);
    PointRef p = PointCreateFromInt(3,2);
    PointRef teta = PointCreateTeta();
	
    CurveRef curve = CurveCreate(mod, a, g);
	
	// Computed points
	PointRef c0 = PointCreateMultiple(p, 0, curve);
	PointRef c1 = PointCreateMultiple(p, 1, curve);
	PointRef c2 = PointCreateMultiple(p, 2, curve);
	PointRef c3 = PointCreateMultiple(p, 3, curve);
    
	// Expected results
	PointRef r2 = PointCreateFromInt(3, 3);
	PointRef r3 = PointCreateFromInt(3, 3);
	
	assert(PointEqual(c0, teta));
	assert(PointEqual(c1, p));
	assert(PointEqual(c2, r2));
	//assert(PointEqual(c3, r3));
	
    mpz_clears(mod, a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    PointDestroy(g);
    PointDestroy(p);
	
	PointDestroy(c0);
	PointDestroy(c1);
	PointDestroy(c2);
	PointDestroy(c3);
	
	PointDestroy(r2);
	PointDestroy(r3);
	PointDestroy(teta);
    
    return 0;
}

