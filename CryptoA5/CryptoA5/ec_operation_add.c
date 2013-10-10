//
//  ec_operation_add.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"
#include <assert.h>

static void lambdaIfEqual(mpz_t lambda, PointRef p, PointRef q, CurveRef curve);
static void lambdaNotEqual(mpz_t lambda, PointRef p, PointRef q, CurveRef curve);
static PointRef add(mpz_t lambda, PointRef p, PointRef q, CurveRef curve);

PointRef PointCreateAdd(PointRef p, PointRef q, CurveRef curve)
{
    PointRef r;
    mpz_t lambda;
    mpz_init(lambda);
    
    if(mpz_cmp(p->x, q->x) == 0)
    {
        lambdaIfEqual(lambda, p, q, curve);
    }
    else
    {
        lambdaNotEqual(lambda, p, q, curve);
    }
    
    r = add(lambda, p, q, curve);
    
    mpz_clear(lambda);
    
    return r;
}

static void lambdaIfEqual(mpz_t lambda, PointRef p, PointRef q, CurveRef curve)
{
    // l = (3.xp^2 + 2.a2.xp + a4 - a1.yp) / (2.yp + a1.xp + a3)
	mpz_t xp, yp;
	mpz_t a1, a2, a3, a4;
	mpz_t mon1, mon2, mon3;
	mpz_t mon4, mon5;
	mpz_t num, denum;
	mpz_t rem;
	
	mpz_init_set(xp, p->x);
	mpz_init_set(yp, p->y);
	mpz_init_set(a1, curve->a[1]);
	mpz_init_set(a2, curve->a[2]);
	mpz_init_set(a3, curve->a[3]);
	mpz_init_set(a4, curve->a[4]);
	mpz_inits(mon1, mon2, mon3, mon4, mon5, NULL);
	mpz_inits(num, denum, NULL);
	mpz_init(rem);
	
	// mon1
	mpz_powm_ui(mon1, xp, 2, curve->mod);
	mpz_mul_ui(mon1, mon1, 3);
	
	// mon2
	mpz_mul(mon2, a2, xp);
	mpz_mul_ui(mon2, mon2, 2);
	
	// mon3
	mpz_mul(mon3, a1, yp);
	
	// num
	mpz_add(num, mon1, mon2);
	mpz_add(num, num, a4);
	mpz_sub(num, num, mon4);
    mpz_mod(num, num, curve->mod);
	
	// mon4
	mpz_mul_ui(mon4, yp, 2);
	
	// mon5
	mpz_mul(mon5, a1, xp);
	
	//denum
	mpz_add(denum, mon4, mon5);
	mpz_add(denum, denum, a3);
    mpz_mod(denum, denum, curve->mod);
	
	// lambda
	mpz_tdiv_qr(lambda, rem, num, denum);
	mpz_mod(lambda, lambda, curve->mod);
    
	// Check rem
	assert(mpz_cmp_si(rem, 0) == 0);
	
	mpz_clears(xp, yp, a1, a2, a3, a4, NULL);
	mpz_clears(mon1, mon2, mon3, mon4, mon5, NULL);
	mpz_clears(num, denum, NULL);
	mpz_clear(rem);
}

static void lambdaNotEqual(mpz_t lambda, PointRef p, PointRef q, CurveRef curve)
{
    mpz_t lambda_rem, lambda_num, lambda_denum;
    mpz_inits(lambda_rem, lambda_num, lambda_denum, NULL);
     
    mpz_sub(lambda_num, p->y, q->y);
    mpz_sub(lambda_denum, p->x, q->x);
     
    mpz_tdiv_qr(lambda, lambda_rem, lambda_num, lambda_denum);
     
    mpz_mod(lambda, lambda, curve->mod);
    
    mpz_clears(lambda_rem, lambda_num, lambda_denum, NULL);
}

static PointRef add(mpz_t lambda, PointRef p, PointRef q, CurveRef curve)
{
    PointRef r = PointCreate();
    
    mpz_t result1, result2;
    mpz_inits(result1, result2, NULL);
    
    mpz_mul(result1, lambda, lambda);
    mpz_sub(result1, result1, p->x);
    mpz_sub(result1, result1, q->x);
    mpz_mod(r->x, result1, curve->mod);
    
    mpz_neg(result1, lambda);
    mpz_mul(result1, result1, r->x);
    mpz_mul(result2, lambda, p->x);
    mpz_add(result1, result1, result2);
    mpz_sub(result1, result1, p->y);
    mpz_mod(r->y, result1, curve->mod);
    
    mpz_clears(result1, result2, NULL);
    
    return r;
}
