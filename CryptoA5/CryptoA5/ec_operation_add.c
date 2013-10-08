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
    PointRef r = PointCreate(p->body);
    mpz_t lambda, result1;
    mpz_inits(lambda, result1, NULL);
    
    if(mpz_cmp(p->x, q->x) == 0)
    {
        lambdaIfEqual(lambda, p, q);
    }
    else
    {
        lambdaNotEqual(lambda, p, q);
    }
    
    /*mpz_t lambda, lambda_rem, lambda_num, lambda_denum, result1;
    mpz_inits(lambda, lambda_num, lambda_denum, result1, NULL);
    
    mpz_sub(lambda_num, p->y, q->y);
    mpz_sub(lambda_denum, p->x, q->x);
    
    mpz_tdiv_qr(lambda, lambda_rem, lambda_num, lambda_denum);
    
    mpz_mod(lambda, lambda, r->body->mod);*/
    
    /*mpz_mul(result1, lambda, lambda);
    mpz_sub(result1, result1, p->x);
    mpz_sub(result1, result1, q->x);
    
    r->*/
    
    return r;
}

static void lambdaIfEqual(mpz_t lambda, PointRef p, PointRef q)
{
    
}

static void lambdaNotEqual(mpz_t lambda, PointRef p, PointRef q)
{
    mpz_t lambda_rem, lambda_num, lambda_denum;
     mpz_inits(lambda_num, lambda_denum, NULL);
     
     mpz_sub(lambda_num, p->y, q->y);
     mpz_sub(lambda_denum, p->x, q->x);
     
     mpz_tdiv_qr(lambda, lambda_rem, lambda_num, lambda_denum);
     
     mpz_mod(lambda, lambda, r->body->mod);

}

static PointRef add(mpz_t lambda, PointRef p, PointRef q)
{
    return NULL;
}
