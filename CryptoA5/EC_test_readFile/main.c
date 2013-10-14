//
//  main.c
//  EC_test_readFile
//
//  Created by Baptiste on 14/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "path.h"
#include "ec_operations.h"
#define FILE_NAME "w256-001.gp"

int main(int argc, const char * argv[])
{
    
    char *filePath = NULL;
	PathInit(argv[0]);
    PathForFile(FILE_NAME, &filePath);
    
    CurveCreateFromFile(filePath);
    

    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

