//
//  main.c
//  EC_test_doublement
//
//  Created by Baptiste on 10/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include <gmp.h>
#include "ec_operations.h"
#include <assert.h>

//p = 5
//lambda = 1
//p = (2,4)
//r = (2,1)
//a4 and a6 = 1
int main(int argc, const char * argv[])
{
    mpz_t a[7], mod;
    mpz_inits(a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    mpz_init_set_si(mod, 5);
    mpz_set_si(a[4], 1);
	mpz_set_si(a[6], 1);
    
    PointRef g = PointCreateFromInt(0,1);
    PointRef p = PointCreateFromInt(3,2);
    
    CurveRef curve = CurveCreate(mod, a, g);
    PointRef r = PointCreateDouble(p, curve);
    
	assert(mpz_cmp_si(r->x, 3) == 0);
	assert(mpz_cmp_si(r->y, 3) == 0);
    
    mpz_clears(mod, a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
    PointDestroy(g);
    PointDestroy(p);
    PointDestroy(r);
    
    return 0;
}

