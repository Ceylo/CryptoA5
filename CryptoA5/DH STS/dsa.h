//
//  dsa.h
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__cipher__
#define __CryptoA5__cipher__

#include <gmp.h>
#include <string>
#include "ec_operations.h"
#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

bool verify_message(const void *data, size_t dataLength, mpz_t u, mpz_t v, CurveRef curve, PointRef pubKey);

void sign_message(const void *data, size_t data_length, mpz_t& u, mpz_t& v, mpz_t secret, CurveRef curve);

#endif /* defined(__CryptoA5__cipher__) */
