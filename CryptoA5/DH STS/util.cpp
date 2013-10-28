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
	
	return result;
}

string readRandomCurve()
{
	long curveId = limited_rand(1, 17);
	char filename[12];
	sprintf(filename, "w256-%03ld.gp", curveId);
	
	return readFile(filename);
}

void sha256(mpz_t h, const string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
	
	mpz_init(h);
	mpz_import (h, 256/8, 1, sizeof(hash[0]), 1, 0, hash);
}

string concatenate(PointRef p, PointRef q) {
    string first_part = PointCreateDescription(p);
    string second_part = PointCreateDescription(q);

   
    return first_part + ";" + second_part;
}