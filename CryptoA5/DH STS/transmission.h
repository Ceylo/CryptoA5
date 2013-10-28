//
//  transmission.h
//  CryptoA5
//
//  Created by Baptiste on 28/10/13.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__transmission__
#define __CryptoA5__transmission__

#include "ec_operations.h"
#include <SFML/Network.hpp>
#include <gmp.h>

using namespace sf;
using namespace std;

PointRef create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA);
void send_key(TcpSocket& stream, PointRef p);
PointRef receive_key(TcpSocket& stream);
string send_random_curve(TcpSocket& stream);
#endif /* defined(__CryptoA5__transmission__) */
