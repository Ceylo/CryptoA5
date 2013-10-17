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
#define FILE_NAME_ERROR "w.gp"

void testReadingFile(const char *path);
void testReadingFileError(const char *path);

int main(int argc, const char * argv[])
{
    const char *path = argv[0];
    
    testReadingFile(path);
    testReadingFileError(path);
	
    return 0;
}

void testReadingFile(const char *path) {
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
	
	CurveDestroy(curve);
	free(filePath);
}

void testReadingFileError(const char *path) {
    char *filePath = NULL;
	PathInit(path);
    PathForFile(FILE_NAME_ERROR, &filePath);
    
	assert(filePath != NULL);
    CurveRef curve = CurveCreateFromFile(filePath);
	
	assert(curve == NULL);
	
	free(filePath);
}

