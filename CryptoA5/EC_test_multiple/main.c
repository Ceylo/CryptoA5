//
//  main.c
//  EC_test_multiple
//
//  Created by Baptiste on 10/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "ec_operations.h"
#include <assert.h>
#include "path.h"

int main(int argc, const char * argv[])
{
	char *filePath = NULL;
	PathInit(argv[0]);
    PathForFile("w256-001.gp", &filePath);
    
	assert(filePath != NULL);
    CurveRef curve = CurveCreateFromFile(filePath);
	assert(curve != NULL);
	
	PointRef g = curve->g;
	mpz_t m;
	mpz_init_set_str(m, "888493310283202167031085660111238327945443791805939712000426466539273165903", 10);
	
	PointRef g2 = PointCreateMultiple(g, m, curve);
	
	mpz_t xr, yr;
	mpz_init_set_str(xr, "5441683091496050436439782524673611468679503009264125966279940185557193452058", 10);
	mpz_init_set_str(yr, "126373273530397135676109694298926901726086297191360274402845796485476517565", 10);
	
	assert(mpz_cmp(g2->x, xr) == 0);
	assert(mpz_cmp(g2->y, yr) == 0);
	
	mpz_set_si(m, 0);
	PointRef g3 = PointCreateMultiple(g, m, curve);
	assert(PointIsTeta(g3));
	
	mpz_set_si(m, 1);
	PointRef g4 = PointCreateMultiple(g, m, curve);
	assert(PointEqual(g4, g));
	
    return 0;
}

