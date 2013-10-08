//
//  ec_operation_add.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"

static void lambdaIfEqual(mpz_t lambda, PointRef p, PointRef q);
static void lambdaNotEqual(mpz_t lambda, PointRef p, PointRef q);
static PointRef add(mpz_t lambda, PointRef p, PointRef q);

PointRef PointCreateAdd(PointRef p, PointRef q)
{
    PointRef r;
    mpz_t lambda;
    mpz_init(lambda);
    
    if(mpz_cmp(p->x, q->x) == 0)
    {
        lambdaIfEqual(lambda, p, q);
    }
    else
    {
        lambdaNotEqual(lambda, p, q);
    }
    
    r = add(lambda, p, q);
    
    mpz_clear(lambda);
    
    return r;
}

static void lambdaIfEqual(mpz_t lambda, PointRef p, PointRef q)
{
    
}

static void lambdaNotEqual(mpz_t lambda, PointRef p, PointRef q)
{
    mpz_t lambda_rem, lambda_num, lambda_denum;
    mpz_inits(lambda_rem, lambda_num, lambda_denum, NULL);
     
    mpz_sub(lambda_num, p->y, q->y);
    mpz_sub(lambda_denum, p->x, q->x);
     
    mpz_tdiv_qr(lambda, lambda_rem, lambda_num, lambda_denum);
     
    mpz_mod(lambda, lambda, p->field->mod);
    
    mpz_clears(lambda_rem, lambda_num, lambda_denum, NULL);
}

static PointRef add(mpz_t lambda, PointRef p, PointRef q)
{
    PointRef r = PointCreate(p->field);
    
    mpz_t result1, result2;
    mpz_inits(result1, result2, NULL);
    
    mpz_mul(result1, lambda, lambda);
    mpz_sub(result1, result1, p->x);
    mpz_sub(result1, result1, q->x);
    mpz_mod(r->x, result1, r->field->mod);
    
    mpz_neg(result1, lambda);
    mpz_mul(result1, result1, r->x);
    mpz_mul(result2, lambda, p->x);
    mpz_add(result1, result1, result2);
    mpz_sub(result1, result1, p->y);
    mpz_mod(r->y, result1, r->field->mod);
    
    mpz_clears(result1, result2, NULL);
    
    return r;
}
