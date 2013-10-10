//
//  ec_operation_invert.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"

PointRef PointCreateInvert(PointRef p, CurveRef curve)
{
    PointRef q = PointCreate();
    mpz_t invYp;
    mpz_init(invYp);
    
    mpz_set(q->x, p->x);
    mpz_mul_si(invYp, p->y, -1);

    mpz_invert(q->y, invYp, curve->mod);
        
    return q;
}
