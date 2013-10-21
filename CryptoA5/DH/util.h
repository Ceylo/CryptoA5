
//
//  util.h
//  DH
//
//  Created by Ceylo on 17/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#include <gmp.h>
#include <string>

using namespace std;

void secure_rand(mpz_t random, mpz_t max);
long limited_rand(long min, long max);
string readFile(string filename);
string readRandomCurve();
