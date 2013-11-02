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

/**
 * Verify the signature of the message from the peer with is public key
 */
bool verify_message(TcpSocket& stream, CurveRef curve, PointRef pubKey);

/**
 * Sign the message and send it to the peer with your private key
 */
void sign_message(TcpSocket& stream, const string& msg, mpz_t secret, CurveRef curve);

#endif /* defined(__CryptoA5__cipher__) */
