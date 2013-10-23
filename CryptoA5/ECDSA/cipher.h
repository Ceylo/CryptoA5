//
//  cipher.h
//  CryptoA5
//
//  Created by Ceylo on 22/10/2013.
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

string decrypt_message(TcpSocket& stream, mpz_t secret, CurveRef curve);

void sign_message(TcpSocket& stream, const string& msg, mpz_t secret, PointRef pubKey, CurveRef curve);

#endif /* defined(__CryptoA5__cipher__) */
