//
//  util.cpp
//  libEC
//
//  Created by Ceylo on 21/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "util.h"
#include <stdlib.h>
#include "path.h"
#include <fstream>
#include <cassert>
#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <iomanip>

void secure_rand(mpz_t random, mpz_t max)
{
	assert(mpz_cmp_si(max, 0) != 0);
	
	// Is that a valid PRNG
	mpz_init(random);
	gmp_randstate_t rstate;
	gmp_randinit_default(rstate);
	gmp_randseed_ui(rstate, rand());
	mpz_urandomm(random, rstate, max);
	gmp_randclear(rstate);
}

long limited_rand(long min, long max)
{
	return min + rand() % (max - min + 1);
}

string readFile(string filename)
{
	char *absoluteFilename = NULL;
	PathForFile(filename.c_str(), &absoluteFilename);
	
	char * memblock;
	string result;
	
	ifstream file (string(absoluteFilename), ios::in|ios::binary|ios::ate);
	free(absoluteFilename);
	
	if (file.is_open())
	{
		size_t size = file.tellg();
		memblock = new char [size+1];
		memset(memblock, 0, size+1);
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		result = memblock;
		delete[] memblock;
	}
	
	return result;
}

string readRandomCurve()
{
	long curveId = limited_rand(1, 17);
	char filename[12];
	sprintf(filename, "w256-%03ld.gp", curveId);
	
	return readFile(filename);
}

void sha256(mpz_t h, const void *data, size_t dataLength)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, dataLength);
    SHA256_Final(hash, &sha256);
	
	mpz_import (h, 256/8, 1, sizeof(hash[0]), 1, 0, hash);
}

string concatenate(PointRef p, PointRef q) {
    char * first_part = PointCreateDescription(p);
    char * second_part = PointCreateDescription(q);
	string concat = string(first_part) + ";" + string(second_part);
    
	free(first_part);
	free(second_part);
	
    return concat;
}

void * pointToKey(PointRef p)
{
	mpz_t hash;
	size_t exportedBytes = 0;
	const size_t sizeInBytes = 256 / 8;
	void *x = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, p->x);
	mpz_init(hash);
	sha256(hash, x, exportedBytes);
	void *buffer = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, hash);
	
	assert(exportedBytes != 0);
	size_t diff = sizeInBytes - exportedBytes;
	
	if (diff > 0)
	{
		void *tmp = malloc(sizeInBytes);
		assert(tmp != NULL);
		
		memset(tmp, 0, sizeInBytes);
		memcpy((char *)tmp + diff, buffer, exportedBytes);
		
		free(buffer);
		buffer = tmp;
	}
	
	free(x);
	mpz_clear(hash);
	
	return buffer;
}

// Algorithm for uv <=> data conversion
//
//mpz_t u, v;
//string us, vs;
//us = string(u);
//vs = string(v);
//
//buffer[us + 1 + vs + 1];
//buffer[0 .. us] = us;
//buffer[us+1] = 0
//buffer[us + 1 .. us + 1 + vs] = vs;
//buffer[us + 1 + vs + 1] = 0
//
//==========
//
//string u = string(buffer);
//buffer += u.size() + 1;
//string v = string(buffer);

void * uvToData(mpz_t u, mpz_t v, size_t& outputLength)
{
	char *uBuffer = NULL;
	char *vBuffer = NULL;
	
	gmp_asprintf(&uBuffer, "%Zd", u);
	gmp_asprintf(&vBuffer, "%Zd", v);
    
    outputLength = strlen(uBuffer) + strlen(vBuffer)+ 2;
    void *concatenateBuffer = malloc(outputLength);
	assert(concatenateBuffer != NULL);
    
    memcpy(concatenateBuffer, uBuffer, strlen(uBuffer));
	memset((char *)concatenateBuffer + strlen(uBuffer), 0, 1);
    memcpy((char *)concatenateBuffer + strlen(uBuffer) + 1, vBuffer, strlen(vBuffer));  //it's now working in my test program
    memset((char *)concatenateBuffer + strlen(uBuffer) + strlen(vBuffer) + 1, 0, 1);
	
    free(uBuffer), uBuffer = NULL;
    free(vBuffer), vBuffer = NULL;
    
    return concatenateBuffer;
}

void dataToUV(const void *data, mpz_t outU, mpz_t outV)
{
	string u = string((char *)data);
	string v = string((char *)data + u.size() + 1);
	mpz_init_set_str(outU, u.c_str(), 10);
	mpz_init_set_str(outV, v.c_str(), 10);
}
