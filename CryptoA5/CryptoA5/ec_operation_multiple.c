//
//  ec_operation_multiple.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"
#include <stdlib.h>
#include <assert.h>



PointRef PointCreateMultiple(PointRef p, mpz_t scalar, CurveRef curve)
{
	assert(p != NULL);
	assert(curve != NULL);
	
	if (mpz_cmp_si(scalar, 0) == 0)
	{
		return PointCreateTeta();
	}
	else if (mpz_cmp_si(scalar, 1) == 0)
	{
		return PointCopy(p);
	}
	
	PointRef q = PointCreateTeta();
	long i;
	int foundBit = 0;
	size_t nbBits = mpz_sizeinbase(scalar, 2);
	
	
	for (i = nbBits - 1; i >= 0;i--)
	{
		int b = mpz_tstbit(scalar, i);
		
		if (b != 0)
			foundBit = 1;
		
		if (foundBit)
		{
			PointRef tmp = PointCreateDouble(q, curve);
			PointDestroy(q);
			q = tmp;
			
			if (b != 0)
			{
				PointRef tmp = PointCreateAdd(q, p, curve);
				PointDestroy(q);
				q = tmp;
			}
		}
	}
	
	return q;
}
