//
//  util.h
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <gmp.h>
#include "ec_operations.h"
#include <string>

#define DHSTS_PORT 4445

using namespace std;

/** Create a random number within [0, max[
 */
void secure_rand(mpz_t random, mpz_t max);

/** Create a small random number withing [min, max[
 */
long limited_rand(long min, long max);

/** Read the given file and return its content
 */
string readFile(string filename);

/** Read some curve file and return its content
 */
string readRandomCurve();

/** Return a GMP hash of str
 */
void sha256(mpz_t h, const void *data, size_t dataLength);

/** Concatenate 2 points and return it like a block of data
 * Between the 2 points there is a ';' to separate them
 */
string concatenate(PointRef p, PointRef q);

/** Returns a binary representation of point @a p
 *
 * Note: the conversion from p to binary is not lossless thus it's not reversible
 *
 * @parameter p the point to convert to binary
 * @return a 256 buffer representing p, you're responsible for freeing it
 */
void * pointToKey(PointRef p);

/** Concatenate 2 mpz_t and return it like a block of data
 */
void* concatenateMpz_t(mpz_t u, mpz_t v, unsigned int* length);
