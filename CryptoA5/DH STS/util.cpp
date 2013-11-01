//
//  util.cpp
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include "util.h"
#include <stdlib.h>
#include "path.h"
#include "ec_operations.h"
#include <fstream>
#include <assert.h>
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
}

long limited_rand(long min, long max)
{
	return min + rand() % (max - min + 1);
}

string readFile(string filename)
{
	char *absoluteFilename = NULL;
	PathForFile(filename.c_str(), &absoluteFilename);
	
	ifstream::pos_type size;
	char * memblock;
	string result;
	
	ifstream file (string(absoluteFilename), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		result = memblock;
		delete[] memblock;
	}
	
    free(absoluteFilename);
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
	
	mpz_init(h);
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



void * pointToKey(PointRef p)
{
	mpz_t hash;
	size_t exportedBytes = 0;
	void *x = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, p->x);
	sha256(hash, x, exportedBytes);
	void *buffer = mpz_export(NULL, &exportedBytes, 1, 1, 1, 0, hash);
	assert(exportedBytes == 256 / 8);
	
    free(x);
    mpz_clear(hash);
	
	return buffer;
}

string concatenateMpz_t(mpz_t u, mpz_t v)
{
    // Send u and v
	char *uBuffer = NULL;
	char *vBuffer = NULL;
	
	gmp_asprintf(&uBuffer, "%Zd", u);
	gmp_asprintf(&vBuffer, "%Zd", v);
    
    char concatenateBuffer[strlen(uBuffer) + strlen(vBuffer)+ 2];
    
    memcpy(concatenateBuffer, uBuffer, strlen(uBuffer));
    
    concatenateBuffer[strlen(uBuffer)] = 0;
    
    memcpy(concatenateBuffer + strlen(uBuffer) + 1, vBuffer, strlen(vBuffer));
    
    concatenateBuffer[sizeof(uBuffer) + sizeof(vBuffer)+ 1] = 0;
    
    string concatenateString = string(concatenateBuffer);
	free(uBuffer), uBuffer = NULL;
	free(vBuffer), vBuffer = NULL;
    
    return concatenateString;
}
