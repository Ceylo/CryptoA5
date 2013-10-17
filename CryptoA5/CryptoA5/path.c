//
//  path.c
//  CryptoA5
//
//  Created by Ceylo on 10/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <stdio.h>
#include "path.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static char *directoryPath = NULL;

void PathInit(const char *executablePath)
{
	assert(executablePath != NULL);
	assert(directoryPath == NULL);
	assert(strlen(executablePath) > 0);
	
	directoryPath = strdup(executablePath);
	int length = (int)strlen(directoryPath);
	size_t position = -1;
	int i;
	
	// Find /
	for (i = length - 1;i >= 0 && position == -1;i--)
	{
		if (directoryPath[i] == '/')
			position = i;
	}
	
	// Erase end
	directoryPath[position+1] = '\0';
}

void PathForFile(const char *path, char **absolutePath)
{
	assert(path != NULL);
	assert(absolutePath != NULL);
	
	size_t pathLen = strlen(path);
	size_t dirPathLen = strlen(directoryPath);
	
	char *result = malloc(pathLen + dirPathLen + 1);
	strcpy(result, directoryPath);
	strcpy(result + dirPathLen, path);
	
	*absolutePath = result;
    free(directoryPath);
    directoryPath = NULL;
}
