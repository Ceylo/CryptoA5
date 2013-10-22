
//
//  util.h
//  DH
//
//  Created by Ceylo on 17/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <gmp.h>
#include <string>

#define EG_PORT 4445

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
void sha256(mpz_t h, const string& str);
