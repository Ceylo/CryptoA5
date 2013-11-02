//
//  transmission.h
//  CryptoA5
//
//  Created by Ceylo on 22/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef __CryptoA5__transmission__
#define __CryptoA5__transmission__

#include "ec_operations.h"
#include <SFML/Network.hpp>
#include <gmp.h>

using namespace sf;
using namespace std;

/** Create the a.P key and send it to the peer
 *
 * The resulting point is returned and the a used to generate the key
 * stored in outA
 */
PointRef create_key(TcpSocket& stream, CurveRef curve, mpz_t& outA);

/** Send the a.P key which is the public key
 */
void send_key(TcpSocket& stream, PointRef p);

/** Receive the remote a.P key
 */
PointRef receive_key(TcpSocket& stream);

/** Send the curve data (raw representation of the curve file) from a random choosen curve
 */
string send_random_curve(TcpSocket& stream);

#endif /* defined(__CryptoA5__transmission__) */
