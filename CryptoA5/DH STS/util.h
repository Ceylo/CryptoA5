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

/** Returns a buffer containing decimal representation of u and v,
 * concatenated and separated by a null character
 *
 * @param u the first mpz_t to encode
 * @param v the second mpz_t to encode
 * @param outputLength the length of the returned buffer, in bytes
 * @return the buffer containing u + null + v + null, you're responsible
 * for freeing this buffer
 */
void * uvToData(mpz_t u, mpz_t v, size_t& outputLength);

/** Extract u and v from the data created by uvToData
 *
 * @param data the data to use for decoding
 * @param u the first mpz_t to decode (output)
 * @param v the second mpz_t to decode (output)
 */
void dataToUV(const void *data, mpz_t outU, mpz_t outV);


