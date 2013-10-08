//
//  ec_operation_add.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"

PointRef PointCreateAdd(PointRef p, PointRef q, mpz_t modulo)
{
    PointRef r = PointCreate();
    
    mpz_t lambda, lambda_rem, lambda_num, lambda_denum;
    mpz_inits(lambda, lambda_num, lambda_denum, NULL);
    
    mpz_sub(lambda_num, p->y, q->y);
    mpz_sub(lambda_denum, p->x, q->x);
    
    mpz_tdiv_qr(lambda, lambda_rem, lambda_num, lambda_denum);
    
    mpz_mod(lambda, lambda, modulo);
    

}
