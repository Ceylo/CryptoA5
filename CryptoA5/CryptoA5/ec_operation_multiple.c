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

static int biggest_fitting_power_of_2(int value)
{
	int k = 0;
	while (1 << (k+1) < value)
		k++;
	
	return k;
}

static void compute_table(PointRef* table, int biggestIndex, CurveRef curve)
{
	int i;
	for (i = 1; i < biggestIndex; i++)
		table[i] = PointCreateDouble(table[i-1], curve);
}

PointRef PointCreateMultiple(PointRef p, int scalar, CurveRef curve)
{
	assert(p != NULL);
	assert(curve != NULL);
	
	if (scalar == 0)
	{
		return PointCopy(PointTeta());
	}
	
	/*
	 
	 find biggest k where 2^k < scalar
	 build table for [p.2^1 .. p.2^k]
	 
	 do
	 {
		Point(result) += table[k]
		scalar -= k
		take biggest k in table where 2^k < scalar
	 } while (k > 0);
	 
	 */
	
	PointRef result = PointCreate();
	
	PointRef* pointTable = NULL;
	int k = biggest_fitting_power_of_2(scalar);
	int tableSize = k;
	pointTable = malloc(sizeof(*pointTable) * tableSize);
	
	// Prepare
	pointTable[0] = PointCopy(p);
	compute_table(pointTable, k, curve);
	
	// Compute
	do {
		// Add and save result
		PointRef tmp = PointCreateAdd(result, pointTable[k], curve);
		PointDestroy(result);
		result = tmp;
		
		// Find next interesting power
		scalar -= k;
		k = biggest_fitting_power_of_2(scalar);
	} while (k > 0);
	
	// Clean
	int i;
	for (i = 0; i < tableSize;i++)
		PointDestroy(pointTable[i]);
	free(pointTable);
	
    return result;
}
