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
    mpz_set(q->x, p->x);
    mpz_invert(q->y, p->x, p->curve->mod);
    
    return q;
}
