//
//  main.c
//  EC_test_path
//
//  Created by Ceylo on 10/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "path.h"
#include <string.h>
#include <assert.h>

#define DIR_PATH "/sdkjhf azfiubs df/zieduh"
#define EXEC_NAME "aizuher"
#define EXEC_PATH DIR_PATH "/" EXEC_NAME
#define FILE_NAME "some curve.gp"

int main(int argc, const char * argv[])
{
	char *filePath = NULL;
	PathInit(EXEC_PATH);
	PathForFile(FILE_NAME, &filePath);
	
	assert(strcmp(DIR_PATH "/" FILE_NAME, filePath) == 0);
    return 0;
}

