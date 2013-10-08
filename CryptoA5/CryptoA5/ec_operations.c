//
//  ec_operations.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

FieldRef FieldCreate(int mod)
{
	assert(mod > 1);
	
	FieldRef newField = malloc(sizeof(*newField));
	assert(newField != NULL);
	
	mpz_init_set_si(newField->mod, mod);
	assert(newField->mod != NULL);
	
	return newField;
}

void FieldDestroy(FieldRef field)
{
	assert(field != NULL);
	
	mpz_clear(field->mod);
	free(field);
}

PointRef PointCreate(FieldRef field)
{
	PointRef newPoint = malloc(sizeof(*newPoint));
    assert(newPoint != NULL);
	
	bzero(newPoint, sizeof((*newPoint)));
	mpz_inits(newPoint->x, newPoint->y, NULL);
	
	assert(newPoint->x != NULL);
	assert(newPoint->y != NULL);
	return newPoint;
}

void PointDestroy(PointRef point)
{
	assert(point != NULL);
	
	mpz_clears(point->x, point->y, NULL);
	free(point);
}

bool FieldEqual(FieldRef aField, FieldRef anotherField)
{
	assert(aField != NULL);
	assert(anotherField != NULL);
	
	if (aField == anotherField ||
		mpz_cmp(aField->mod, anotherField->mod) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PointEqual(PointRef p, PointRef q)
{
	assert(p != NULL);
	assert(q != NULL);
	
	if (mpz_cmp(p->x, q->x) == 0 &&
		mpz_cmp(p->y, q->y) == 0 &&
		FieldEqual(p->field, q->field))
	{
		return true;
	}
	else
	{
		return false;
	}
}
