//
//  main.c
//  EC_test_readFile
//
//  Created by Baptiste on 14/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "path.h"
#include "ec_operations.h"
#define FILE_NAME "w256-001.gp"

int main(int argc, const char * argv[])
{
    char *filePath = NULL;
	PathInit(argv[0]);
    PathForFile(FILE_NAME, &filePath);
    
	assert(filePath != NULL);
    CurveRef curve = CurveCreateFromFile(filePath);
	
	assert(curve != NULL);
	assert(curve->a[4] != NULL);
	assert(curve->a[6] != NULL);
	assert(curve->g != NULL);
	assert(curve->mod != NULL);
	
	CurveDestroy(curve);
	free(filePath);
	
    return 0;
}

