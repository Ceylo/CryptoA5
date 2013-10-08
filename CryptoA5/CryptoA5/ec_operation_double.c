//
//  ec_operation_double.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"
#include <assert.h>

PointRef PointCreateDouble(PointRef p)
{
	assert(p != NULL);
	return PointCreateAdd(p, p);
}
