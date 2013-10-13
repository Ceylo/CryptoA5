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
	// -P = (xp, - yp - a1.xp - a3)
	
	mpz_t mon;
	
	// mon = a1.xp
	mpz_init_set(mon, curve->a[1]);
	mpz_mul(mon, mon, q->x);
	mpz_mod(mon, mon, curve->mod);
	
	// yp = -yp
	mpz_neg(q->y, q->y);
	mpz_mod(q->y, q->y, curve->mod);
	
	// yp = -yp - a1.xp
	mpz_sub(q->y, q->y, mon);
	mpz_mod(q->y, q->y, curve->mod);
	
	// yp = -yp - a1.xp - a3
	mpz_sub(q->y, q->y, curve->a[3]);
	mpz_mod(q->y, q->y, curve->mod);
	
	mpz_clear(mon);
	
    return q;
}
