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



PointRef PointCreateMultiple(PointRef p, int scalar, CurveRef curve)
{
	assert(p != NULL);
	assert(curve != NULL);
	
	if (scalar == 0)
	{
		return PointCreateTeta();
	}
	else if (scalar == 1)
	{
		return PointCopy(p);
	}

	PointRef q = PointCreateTeta();
	int i;
	int foundBit = 0;
	
	for (i = sizeof(scalar) * 8 - 1; i >= 0;i--)
	{
		int b = scalar & (1 << i);
		
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
