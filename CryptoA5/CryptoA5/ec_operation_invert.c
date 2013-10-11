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
    PointRef q = PointCopy(p);
	
	//	P = (xp, yp)
	// -P = (xp, yp + xp)
	mpz_add(q->y, q->y, q->x);
	
    return q;
}
