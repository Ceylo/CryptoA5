//
//  main.c
//  EC_test_path
//
//  Created by Ceylo on 10/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "path.h"

int main(int argc, const char * argv[])
{
	char *filePath = NULL;
	PathInit(argv[0]);
	
	PathForFile("some curve.gp", &filePath);
	
	printf(">> %s\n", filePath);
	
    return 0;
}

