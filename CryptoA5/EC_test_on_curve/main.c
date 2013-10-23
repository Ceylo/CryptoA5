//
//  main.c
//  EC_test_on_curve
//
//  Created by Baptiste on 23/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "path.h"
#include "ec_operations.h"
#define FILE_NAME "w256-001.gp"

CurveRef readingFile(const char *path);

int main(int argc, const char * argv[])
{
    const char *path = argv[0];
    mpz_t scalar;
    mpz_init_set_ui(scalar, 30);
    CurveRef curve = readingFile(path);
    
    assert(PointIsOnCurve(curve->g, curve));
    
    PointRef p = PointCreateMultiple(curve->g, scalar, curve);
    
    assert(PointIsOnCurve(p, curve));

    
    CurveDestroy(curve);
        
    return 0;
}

CurveRef readingFile(const char *path) {
    char *filePath = NULL;
	PathInit(path);
    PathForFile(FILE_NAME, &filePath);
    
	assert(filePath != NULL);
    CurveRef curve = CurveCreateFromFile(filePath);
	
	assert(curve != NULL);
	assert(curve->a[4] != NULL);
	assert(curve->a[6] != NULL);
	assert(curve->g != NULL);
	assert(curve->mod != NULL);
	
	free(filePath);
    return curve;
}



