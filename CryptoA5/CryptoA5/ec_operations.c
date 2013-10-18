//
//  ec_operations.c
//  CryptoA5
//
//  Created by Ceylo on 04/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "ec_operations.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

CurveRef CurveCreate(mpz_t mod, mpz_t a[7], PointRef g)
{
	// Check input
	assert(mod != NULL);
	assert(g != NULL);
	
	// Create structure
	CurveRef curve = malloc(sizeof(*curve));
	assert(curve != NULL);
	
	// Init members
	mpz_init_set(curve->mod, mod);
	
	int i;
	for (i = 0; i < 7;i++)
	{
		mpz_init_set(curve->a[i], a[i]);
	}
	curve->g = PointCopy(g);
	
	// Check members
	assert(curve->mod != NULL);
	assert(curve->g != NULL);
	
	return curve;
}

//CurveRef CurveCreateFromData(const char *data)
//{
//	
//}

CurveRef CurveCreateFromFile(const char *filename)
{
    FILE *input = fopen(filename, "r");
    CurveRef curve = NULL;
	
    if(input == NULL)
    {
        perror("Error opening file");
    }
    else
    {
        mpz_t p, a4, a6, gx, gy, n, r4, r6;
        mpz_t a[7];
        PointRef g = PointCreate();
        
        mpz_inits(p, gx, gy, a4, a6, n, r4, r6, NULL);
        mpz_inits(a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
        
        gmp_fscanf(input, "p=%Zd\n", p);
        gmp_fscanf(input, "n=%Zd\n", n);
        gmp_fscanf(input, "a4=%Zd\n", a[4]);
        gmp_fscanf(input, "a6=%Zd\n", a[6]);
        gmp_fscanf(input, "r4=%Zd\n", r4);
        gmp_fscanf(input, "r6=%Zd\n", r6);
        gmp_fscanf(input, "gx=%Zd\n", g->x);
        gmp_fscanf(input, "gy=%Zd\n", g->y);
        
#if DEBUG
        gmp_printf("p : %Zd\n", p);
        gmp_printf("n : %Zd\n", n);
        gmp_printf("a4 : %Zd\n", a[4]);
        gmp_printf("a6 : %Zd\n", a[6]);
//        gmp_printf("r4 : %Zd\n", r4);
//        gmp_printf("r6 : %Zd\n", r6);
        gmp_printf("g->x : %Zd\n", g->x);
        gmp_printf("g->y : %Zd\n", g->y);
#endif
        
        curve = CurveCreate(p, a, g);
		
		mpz_clears(p, gx, gy, a4, a6, n, r4, r6, NULL);
		mpz_clears(a[0], a[1], a[2], a[3], a[4], a[5], a[6], NULL);
		
		fclose(input);
    }
	
	return curve;
}

void CurveDestroy(CurveRef curve)
{
	// Check input
	assert(curve != NULL);
	
	// Free data
	mpz_clear(curve->mod);
	PointDestroy(curve->g);
	free(curve);
}

bool CurveEqual(CurveRef aCurve, CurveRef anotherCurve)
{
	assert(aCurve != NULL);
	assert(anotherCurve != NULL);
	
	if (aCurve == anotherCurve ||
		(mpz_cmp(aCurve->mod, anotherCurve->mod) == 0 &&
		 memcmp(aCurve->a, anotherCurve->a, sizeof(int[7])) == 0 &&
		 PointEqual(aCurve->g, anotherCurve->g)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

PointRef PointCreateTeta()
{
	PointRef p = PointCreate();
	p->infinite = true;
	return p;
}

bool PointIsTeta(PointRef p)
{
	assert(p != NULL);
	
	return p->infinite;
}

PointRef PointCreate()
{
	PointRef newPoint = malloc(sizeof(*newPoint));
    assert(newPoint != NULL);
	
	bzero(newPoint, sizeof((*newPoint)));
	mpz_inits(newPoint->x, newPoint->y, NULL);
	
	assert(newPoint->x != NULL);
	assert(newPoint->y != NULL);
	return newPoint;
}

PointRef PointCreateFromGMP(mpz_t x, mpz_t y)
{
	assert(x != NULL);
	assert(y != NULL);
	
	PointRef point = PointCreate();
	assert(point != NULL);
	
	mpz_set(point->x, x);
	mpz_set(point->y, y);
	
	return point;
}

PointRef PointCreateFromInt(int x, int y)
{
	mpz_t gmpx, gmpy;
	mpz_init_set_si(gmpx, x);
	mpz_init_set_si(gmpy, y);
	
	PointRef point = PointCreateFromGMP(gmpx, gmpy);
	mpz_clears(gmpx, gmpy, NULL);
	
	return point;
}

char *			PointCreateDescription(PointRef p)
{
	assert(p != NULL);
	char *buffer = NULL;
	
	if (p->infinite)
		gmp_asprintf(&buffer, "%p = teta", p);
	else
		gmp_asprintf(&buffer, "%p = (%Zd, %Zd)", p, p->x, p->y);
	
	return buffer;
}

PointRef PointCopy(PointRef other)
{
	assert(other != NULL);
	
	PointRef point = PointCreate();
	mpz_set(point->x, other->x);
	mpz_set(point->y, other->y);
	point->infinite = other->infinite;
	
	return point;
}

void PointDestroy(PointRef point)
{
	assert(point != NULL);
	
	mpz_clears(point->x, point->y, NULL);
	free(point);
}

bool PointEqual(PointRef p, PointRef q)
{
	assert(p != NULL);
	assert(q != NULL);
	
	if (mpz_cmp(p->x, q->x) == 0 &&
		mpz_cmp(p->y, q->y) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PointCongruent(PointRef p, PointRef q, CurveRef curve)
{
	assert(p != NULL);
	assert(q != NULL);
	assert(curve != NULL);
	
	bool result = false;
	
	mpz_t xp, yp, xq, yq;
	mpz_inits(xp, yp, xq, yq, NULL);
	
	mpz_mod(xp, p->x, curve->mod);
	mpz_mod(yp, p->y, curve->mod);
	mpz_mod(xq, q->x, curve->mod);
	mpz_mod(yq, q->y, curve->mod);
	
	int xeq = mpz_cmp(xp, xq);
	int yeq = mpz_cmp(yp, yq);
	
	if (xeq == 0 && yeq == 0)
	{
		result = true;
	}
	
	mpz_clears(xp, yp, xq, yq);
	return result;
}

